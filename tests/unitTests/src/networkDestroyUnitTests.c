#include <criterion/criterion.h>
#include "networkDestroy.h"

Test(simple5, test)
{
	cr_assert(1);
	cr_expect(strlen("Test") == 4, "Expected \"Test\" to have a length of 4.");
    //cr_expect(strlen("Hello") == 4, "This will always fail, why did I add this?");
    cr_assert(strlen("") == 0);
}

Test(simple6, test)
{
	cr_assert(1);
	cr_expect(strlen("Test") == 4, "Expected \"Test\" to have a length of 4.");
    //cr_expect(strlen("Hello") == 4, "This bananas, why did I add this?");
    cr_assert(strlen("") == 0);
}

Test(simple7, test)
{
	cr_assert(1);
	cr_expect(strlen("Test") == 4, "Expected \"Test\" to have a length of 4.");
    //cr_expect(strlen("Hello") == 4, "This bananas, why did I add this?");
    cr_assert(strlen("") == 0);
}