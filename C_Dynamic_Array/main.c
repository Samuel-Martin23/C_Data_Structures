#include "dynamic_array_int.h"

int main()
{
    DynamicArray *nums = dynamic_array_init_int();

    for (int i = 0; i < 50; i++)
    {
        dynamic_array_append_int(nums, i);
    }

    dynamic_array_pop_index_range(nums, 5, 40, 1);

    printf("The sum of the array is %d.\n", dynamic_array_sum_int(nums));

    dynamic_array_extend_int_args(nums, 4, 100, 101, 102, 103);

    dynamic_array_remove_int(nums, 100);

    dynamic_array_print(nums);

    dynamic_array_free(nums);

    return 0;
}
