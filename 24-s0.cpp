//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: 24-s0.cpp     Fri, Nov  7 07:09:18 2003 +0800  meihui $ *
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

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <stdio.h>

struct Value {
    int x, y;
    std::string s;	// v = x / y;

    explicit Value(int a): x(a), y(1) {
	char buf[128 / 3 + 2];
	sprintf(buf, "%d", a);
	s = buf;
    }

    Value(int a, int b, std::string const& c): x(a), y(b), s(c) {}

    Value operator+ (Value const& r) const {
	return Value(x * r.y + y * r.x, y * r.y, op("+", r));
    }
    Value operator- (Value const& r) const {
	return Value(x * r.y - y * r.x, y * r.y, op("-", r));
    }
    Value operator* (Value const& r) const {
	return Value(x * r.x, y * r.y, op("*", r));
    }
    Value operator/ (Value const& r) const {
	return Value(x * r.y, r.y ? y * r.x : 0, op("/", r));
    }

    bool  operator==(Value const& r) const {
	return y != 0 && r.y != 0 && x * r.y == r.x * y;
    }

private:
    std::string op(char const* sop, Value const& r) const {
	return "(" + s + sop + r.s + ")";
    }
};

void solve(std::vector<Value> const& s, const Value& g)
{
    if (s.size() == 1 && s.front() == g)
	std::cout << s.front().s << '\n';

    for (size_t i = 0; i < s.size() - 1; ++i) {
	for (size_t j = i + 1; j < s.size(); ++j) {
	    const Value &vi = s[i], &vj = s[j];
	    std::vector<Value> t = s;		// low-performance
	    t.erase(t.begin() + std::max(i,j));	// for high-performance
	    t.erase(t.begin() + std::min(i,j));	// for high-performance
	    // if (vi < vj) continue;
	    t.push_back(vi + vj);	solve(t, g);
	    t.back() = vi * vj;		solve(t, g);
	    t.back() = vi - vj;		solve(t, g);
	    t.back() = vi / vj;		solve(t, g);
	    t.back() = vj - vi;		solve(t, g);
	    t.back() = vj / vi;		solve(t, g);
	}
    }
}

int main()
{
    const char* s0 = "\033[0;33mPlease input the data as: "
		     "\033[1;4;31mgoal  num1 num2 ...\033[0;1;35m";

    std::cout << "\033[2J\033[0;0H" << s0 << std::endl;	std::string str;

    while (std::getline(std::cin, str)) {
	std::istringstream is(str);
	std::vector<Value> s;
	int v, g;

	if (!(is >> g)) continue;
	while (is >> v) s.push_back(Value(v));

	std::cout << std::endl << "\033[37mGot the results: \033[32m"
		  << std::endl;

	solve(s, Value(g));

	std::cout << std::endl << s0 << std::endl;
	is.clear();
    }
}

/* Build instructions:
    g++ -Wall -pipe -O2 -o 24-s0{,.cpp}
 */
// vim:sts=4:ts=8:
