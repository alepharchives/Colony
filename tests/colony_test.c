/*
    Copyright (C) 2011 Colony authors,
    
    This file is part of Colony.
    
    Colony is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Colony is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdbool.h>

#include "colony_test.h"
#include "colony.h"

void test_function(void *arg) {
    bool *ret = arg;
    *ret = true;
}

START_TEST(test_1) {
    thread_pool *pool = thread_pool_new(2);
    
    bool a=false, b=false;
    
    thread_pool_run(pool, test_function, &a);
    thread_pool_run(pool, test_function, &b);
    
    thread_pool_free(pool);
    
    fail_unless(a, NULL);
    fail_unless(b, NULL);
} END_TEST

TCase *colony_test_case() {
    TCase *tc = tcase_create("colony");
    tcase_add_test(tc, test_1);
    return tc;
}
