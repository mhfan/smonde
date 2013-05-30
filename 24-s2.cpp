/****************************************************************
 * $ID: 24-s2.cpp     Fri, Nov  7 07:25:02 2003 +0800  meihui $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer:  ∑∂√¿ª‘(MeiHui FAN)  <mhfan@ustc.edu>            *
 *              Laboratory of Structural Biology                *
 *              School of Life Science                          *
 *              Univ. of Sci.& Tech. of China (USTC)            *
 *              People's Republic of China (PRC)                *
 *                                                              *
 * CopyLeft (c)  2003~2013  M.H.Fan                             *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it              *
 *   under the terms of the GNU General Public Licence (GPL).   *
 *                                                              *
 * No warranty, no liability; use this at your own risk!        *
 ****************************************************************/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <set>

#include <stdio.h>

#include "smonde.h"

using Smonde::Product;
using Smonde::Sum;

struct Value {
    int x, y;
    std::string s;	// v = x/y;

    explicit Value(int a): x(a), y(1) {
	char buf[64];
	sprintf(buf, "%d", a);
	s = buf;
    }

    Value(int a, int b, std::string const& c, char o):
	    x(a), y(b), s(c), ops(o) {}

    Value operator+ (Value const& r) const {
	return Value(x * r.y + y * r.x, y * r.y, op('+', r), '+');
    }
    Value operator- (Value const& r) const {
	return Value(x * r.y - y * r.x, y * r.y, op('-', r), '+');
    }
    Value operator* (Value const& r) const {
	return Value(x * r.x, y * r.y, op('*', r), '*');
    }
    Value operator/ (Value const& r) const {
	return Value(x * r.y, r.y ? y * r.x : 0, op('/', r), '*');
    }

    bool valid() const { return y != 0; }

    bool operator==(Value const& r) const {
	return valid() && r.valid() && x * r.y == r.x * y;
    }

private:
    char ops;

    std::string op(char const sop, Value const& r) const {
	//return '(' + s + sop + r.s + ')';

	char a, b, c, d;
	a = b = c = d = '\0';

	if (sop == '*' || sop == '/') {
	    if (  ops == '+') a = '(', c = ')';
	    if (r.ops == '+') b = '(', d = ')';
	}

	return a + s + c + sop + b + r.s + d;
    }
};

typedef Value (Value::* Operator)(Value const&) const;

struct Universe {
    std::vector<int> nums;
    std::stack<Value> vals;
    std::stack<Operator> ops;
    std::stack<std::string> subs;
};

typedef Smonde::Multiverse<Universe> Multiverse;

struct Num {
    static void apply(Universe& u, Multiverse& m) {
	for (size_t i = 0; i < u.nums.size(); ++i) {
	    Universe v(u);
	    v.vals.push(Value(v.nums[i]));
	    v.nums[i] = v.nums.back();
	    v.nums.pop_back();
	    m.result(v);
	}
    }
};

template <Operator op>
struct OP {
    static void apply(Universe& u, Multiverse& m) {
	if (u.ops.size() + 1 < u.nums.size() + u.vals.size()) {
	    u.ops.push(op);
	    m.result(u);
	}
    }
};

struct Reduce {
    static void apply(Universe& u, Multiverse& m) {
	Value v = u.vals.top();
	u.vals.pop();
	u.vals.top() = (u.vals.top().* u.ops.top())(v);
	u.ops.pop();
	m.result(u);
    }
};

struct Push_sub {
    static void apply(Universe& u, Multiverse& m) {
	u.subs.push(u.vals.top().s);
	m.result(u);
    }
};

struct Pop_sub {
    static void apply(Universe& u, Multiverse& m) {
	u.subs.pop();
	m.result(u);
    }
};

struct Check_order {
    static void apply(Universe& u, Multiverse& m) {
	if (u.vals.top().s < u.subs.top()) return;
	u.subs.top() = u.vals.top().s;
	m.result(u);
    }
};

template <Operator op, typename S>
struct Nform1_: Sum<Product<OP<op>, S, Push_sub,   S, Check_order, Reduce>,
		    Product<OP<op>, Nform1_<op,S>, S, Check_order, Reduce> >
{};

template <Operator op, typename Sform>
struct Nform1: Product<Nform1_<op, Sum<Num, Sform> >, Pop_sub> {};

template <Operator op, Operator sop, typename Sform>
struct Nform2: Product<OP<sop>, Sum<Num, Sform, Nform1<op, Sform> >,
				 Sum<Num, Sform, Nform1<op, Sform> >,
				 Reduce>
{};

template <Operator op, Operator sop, typename Sform>
struct Nform: Sum<Nform1<op, Sform>, Nform2<op, sop, Sform> > {};

#if __BORLANDC__
struct Addsubform: Nform<&Value::operator+, &Value::operator-,
			 Nform<&Value::operator*, &Value::operator/,
			 Addsubform> >
{};

struct Muldivform: Nform<&Value::operator*, &Value::operator/, Addsubform> {};
#else
template <typename Addsubform>
struct Muldivfor_: Nform<&Value::operator*, &Value::operator/, Addsubform> {};

struct Addsubform: Nform<&Value::operator+, &Value::operator-,
			 Muldivfor_<Addsubform> >
{};

struct Muldivform: Muldivfor_<Addsubform> {};
#endif

#if DEBUG
template <typename Rule>
struct Inspect {
    template <typename Universe>
    static void apply (Universe& u, Multiverse<Universe>& m) {
	std::cerr << "\033[1m" << typeid(Rule).name()
		  << ": \033[36mnums=\033[33m" << u.nums.size()
		  << ", \033[36mvals=\033[33m" << u.vals.size()
		  << ", \033[36m ops=\033[33m" << u.ops.size()
		  << "\033[0;37m." << std::endl;
	Rule::apply(u, m);
    }
};

struct Expr: Sum<Num, Addsubform, Inspect<Muldivform> > {};
#else
struct Expr: Sum<Num, Addsubform, Muldivform> {};
#endif

/*
struct Any_op: Sum<OP<&Value::operator+ >, OP<&Value::operator- >,
		   OP<&Value::operator* >, OP<&Value::operator/ > > {};
struct Expr  : Sum<Num, Product<Any_op, Expr, Expr, Reduce> > {};
//struct Expr  : Sum<Num, Product<Any_op, Expr, Expr, Inspect<Reduce> > > {};
 */

struct Destiny: Multiverse {
    Destiny(int g): goal_(g) {}

    void result(Universe& u) {
	if (u.nums.size() != 0) return;
	Value const& v = u.vals.top();
	if (v == Value(goal_) && seen_.insert(v.s).second)
	    std::cout << v.s << "\n";
    }

private:
    int goal_;
    std::set<std::string> seen_;
};

int main()
{
    const char* s0 = "\033[0;33mPlease type the data as: "
		     "\033[1;4;31mgoal  num1 num2 ...\033[0;1;35m";

    std::cout << "\033[2J\033[0;0H" << s0 << std::endl;
    std::string str;

    while (std::getline(std::cin, str)) {
	Universe universe;
	int g, n;

	std::istringstream is(str);
	if (!(is >> g)) continue;
	while (is >> n) universe.nums.push_back(n);

	std::cout << std::endl << "\033[37mGot the results: \033[32m"
		  << std::endl;

	Destiny destiny(g);
	Expr::apply(universe, destiny);

	std::cout << std::endl << s0 << std::endl;
	is.clear();
    }
}

/* Build instructions:
    g++ -Wall -pipe -O2 -o 24-s2{,.cpp}
 */
// vim:sts=4:ts=8:
