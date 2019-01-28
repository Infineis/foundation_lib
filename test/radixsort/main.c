/* main.c  -  Foundation radixsort test  -  Public Domain  -  2013 Mattias Jansson / Rampant Pixels
 *
 * This library provides a cross-platform foundation library in C11 providing basic support
 * data types and functions to write applications and games in a platform-independent fashion.
 * The latest source code is always available at
 *
 * https://github.com/rampantpixels/foundation_lib
 *
 * This library is put in the public domain; you can redistribute it and/or modify it without
 * any restrictions.
 */

#include <foundation/foundation.h>
#include <test/test.h>

static application_t
test_radixsort_application(void) {
	application_t app;
	memset(&app, 0, sizeof(app));
	app.name = string_const(STRING_CONST("Foundation radixsort tests"));
	app.short_name = string_const(STRING_CONST("test_radixsort"));
	app.company = string_const(STRING_CONST("Rampant Pixels"));
	app.flags = APPLICATION_UTILITY;
	app.exception_handler = test_exception_handler;
	return app;
}

static memory_system_t
test_radixsort_memory_system(void) {
	return memory_system_malloc();
}

static foundation_config_t
test_radixsort_config(void) {
	foundation_config_t config;
	memset(&config, 0, sizeof(config));
	return config;
}

static int
test_radixsort_initialize(void) {
	return 0;
}

static void
test_radixsort_finalize(void) {
}

DECLARE_TEST(radixsort, allocation) {
	radixsort_t* sort_none = radixsort_allocate(RADIXSORT_INT32, 0);
	radixsort_t* sort_small = radixsort_allocate(RADIXSORT_INT32, 128);
	radixsort_t* sort_large = radixsort_allocate(RADIXSORT_INT32,
	                                             (radixsort_index_t)((1ULL << (uint64_t)(sizeof(radixsort_index_t) * 8)) - 1));

	EXPECT_NE(sort_none, 0);
	EXPECT_NE(sort_small, 0);
	EXPECT_NE(sort_large, 0);

	radixsort_deallocate(sort_none);
	radixsort_deallocate(sort_small);
	radixsort_deallocate(sort_large);

	return 0;
}

DECLARE_TEST(radixsort, sort_int32) {
	int bits = 0;
	int max_bits = 20;
	int num_bits = sizeof(radixsort_index_t) * 8;
	int32_t* arr_int;
	uint32_t* arr_uint;
	radixsort_t* sort_int;
	radixsort_t* sort_uint;

	if (num_bits > max_bits)
		num_bits = max_bits;

	sort_int = radixsort_allocate(RADIXSORT_INT32,
	                              (radixsort_index_t)((1ULL << (uint64_t)num_bits) - 1));
	sort_uint = radixsort_allocate(RADIXSORT_UINT32,
	                               (radixsort_index_t)((1ULL << (uint64_t)num_bits) - 1));

	for (bits = 1; bits <= num_bits; ++bits) {
		radixsort_index_t ival, sval;
		radixsort_index_t num = (radixsort_index_t)((1ULL << (uint64_t)bits) - 1);
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_int;
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_uint;

		arr_int = memory_allocate(0, sizeof(int32_t) * num, 0, MEMORY_PERSISTENT);
		arr_uint = memory_allocate(0, sizeof(uint32_t) * num, 0, MEMORY_PERSISTENT);

		for (ival = 0; ival < num; ++ival) {
			arr_int[ival] = (int32_t)random32();
			arr_uint[ival] = random32();
		}

		sindex_int = radixsort_sort(sort_int, arr_int, num);
		sindex_uint = radixsort_sort(sort_uint, arr_uint, num);

		if (num == 1) {
			EXPECT_EQ(sindex_int[0], 0);
			EXPECT_EQ(sindex_uint[0], 0);
		}
		else for (ival = 1; ival < num; ++ival) {
				for (sval = 0; sval < ival; ++sval) {
					EXPECT_NE(sindex_int[sval], sindex_int[ival]);
					EXPECT_NE(sindex_uint[sval], sindex_uint[ival]);
				}
				EXPECT_LE(arr_int[ sindex_int[ival - 1] ], arr_int[ sindex_int[ival] ]);
				EXPECT_LE(arr_uint[ sindex_uint[ival - 1] ], arr_uint[ sindex_uint[ival] ]);
			}

		memory_deallocate(arr_int);
		memory_deallocate(arr_uint);
	}

	for (; bits >= 1; --bits) {
		radixsort_index_t ival, sval;
		radixsort_index_t num = (radixsort_index_t)((1ULL << (uint64_t)bits) - 1);
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_int;
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_uint;

		arr_int = memory_allocate(0, sizeof(int32_t) * num, 0, MEMORY_PERSISTENT);
		arr_uint = memory_allocate(0, sizeof(uint32_t) * num, 0, MEMORY_PERSISTENT);

		for (ival = 0; ival < num; ++ival) {
			arr_int[ival] = (int32_t)random32();
			arr_uint[ival] = random32();
		}

		sindex_int = radixsort_sort(sort_int, arr_int, num);
		sindex_uint = radixsort_sort(sort_uint, arr_uint, num);

		if (num == 1) {
			EXPECT_EQ(sindex_int[0], 0);
			EXPECT_EQ(sindex_uint[0], 0);
		}
		else for (ival = 1; ival < num; ++ival) {
				for (sval = 0; sval < ival; ++sval) {
					EXPECT_NE(sindex_int[sval], sindex_int[ival]);
					EXPECT_NE(sindex_uint[sval], sindex_uint[ival]);
				}
				EXPECT_LE(arr_int[ sindex_int[ival - 1] ], arr_int[ sindex_int[ival] ]);
				EXPECT_LE(arr_uint[ sindex_uint[ival - 1] ], arr_uint[ sindex_uint[ival] ]);
			}

		memory_deallocate(arr_int);
		memory_deallocate(arr_uint);
	}

	radixsort_deallocate(sort_int);
	radixsort_deallocate(sort_uint);

	return 0;
}

DECLARE_TEST(radixsort, sort_int64) {
	int bits = 0;
	int max_bits = 20;
	int num_bits = sizeof(radixsort_index_t) * 8;
	int64_t* arr_int;
	uint64_t* arr_uint;
	radixsort_t* sort_int;
	radixsort_t* sort_uint;

	if (num_bits > max_bits)
		num_bits = max_bits;

	sort_int = radixsort_allocate(RADIXSORT_INT64,
	                              (radixsort_index_t)((1ULL << (uint64_t)num_bits) - 1));
	sort_uint = radixsort_allocate(RADIXSORT_UINT64,
	                               (radixsort_index_t)((1ULL << (uint64_t)num_bits) - 1));

	for (bits = 1; bits <= num_bits; ++bits) {
		radixsort_index_t ival, sval;
		radixsort_index_t num = (radixsort_index_t)((1ULL << (uint64_t)bits) - 1);
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_int;
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_uint;

		arr_int = memory_allocate(0, sizeof(int64_t) * num, 0, MEMORY_PERSISTENT);
		arr_uint = memory_allocate(0, sizeof(uint64_t) * num, 0, MEMORY_PERSISTENT);

		for (ival = 0; ival < num; ++ival) {
			arr_int[ival] = (int64_t)random64();
			arr_uint[ival] = random64();
		}

		sindex_int = radixsort_sort(sort_int, arr_int, num);
		sindex_uint = radixsort_sort(sort_uint, arr_uint, num);

		if (num == 1) {
			EXPECT_EQ(sindex_int[0], 0);
			EXPECT_EQ(sindex_uint[0], 0);
		}
		else for (ival = 1; ival < num; ++ival) {
				for (sval = 0; sval < ival; ++sval) {
					EXPECT_NE(sindex_int[sval], sindex_int[ival]);
					EXPECT_NE(sindex_uint[sval], sindex_uint[ival]);
				}
				EXPECT_LE(arr_int[ sindex_int[ival - 1] ], arr_int[ sindex_int[ival] ]);
				EXPECT_LE(arr_uint[ sindex_uint[ival - 1] ], arr_uint[ sindex_uint[ival] ]);
			}

		memory_deallocate(arr_int);
		memory_deallocate(arr_uint);
	}

	for (; bits >= 1; --bits) {
		radixsort_index_t ival, sval;
		radixsort_index_t num = (radixsort_index_t)((1ULL << (uint64_t)bits) - 1);
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_int;
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_uint;

		arr_int = memory_allocate(0, sizeof(int64_t) * num, 0, MEMORY_PERSISTENT);
		arr_uint = memory_allocate(0, sizeof(uint64_t) * num, 0, MEMORY_PERSISTENT);

		for (ival = 0; ival < num; ++ival) {
			arr_int[ival] = (int64_t)random64();
			arr_uint[ival] = random64();
		}

		sindex_int = radixsort_sort(sort_int, arr_int, num);
		sindex_uint = radixsort_sort(sort_uint, arr_uint, num);

		if (num == 1) {
			EXPECT_EQ(sindex_int[0], 0);
			EXPECT_EQ(sindex_uint[0], 0);
		}
		else for (ival = 1; ival < num; ++ival) {
				for (sval = 0; sval < ival; ++sval) {
					EXPECT_NE(sindex_int[sval], sindex_int[ival]);
					EXPECT_NE(sindex_uint[sval], sindex_uint[ival]);
				}
				EXPECT_LE(arr_int[ sindex_int[ival - 1] ], arr_int[ sindex_int[ival] ]);
				EXPECT_LE(arr_uint[ sindex_uint[ival - 1] ], arr_uint[ sindex_uint[ival] ]);
			}

		memory_deallocate(arr_int);
		memory_deallocate(arr_uint);
	}

	radixsort_deallocate(sort_int);
	radixsort_deallocate(sort_uint);

	return 0;
}

DECLARE_TEST(radixsort, sort_real) {
	int bits = 0;
	int max_bits = 20;
	int num_bits = sizeof(radixsort_index_t) * 8;
	float32_t* arr_32;
	float64_t* arr_64;
	radixsort_t* sort_32;
	radixsort_t* sort_64;
	real low_range = -(real)(1 << 30);
	real high_range = (real)(1 << 30);

	if (num_bits > max_bits)
		num_bits = max_bits;

	sort_32 = radixsort_allocate(RADIXSORT_FLOAT32,
	                             (radixsort_index_t)((1ULL << (uint64_t)num_bits) - 1));
	sort_64 = radixsort_allocate(RADIXSORT_FLOAT64,
	                             (radixsort_index_t)((1ULL << (uint64_t)num_bits) - 1));

	for (bits = 1; bits <= num_bits; ++bits) {
		radixsort_index_t ival, sval;
		radixsort_index_t num = (radixsort_index_t)((1ULL << (uint64_t)bits) - 1);
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_32;
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_64;

		arr_32 = memory_allocate(0, sizeof(float32_t) * num, 0, MEMORY_PERSISTENT);
		arr_64 = memory_allocate(0, sizeof(float64_t) * num, 0, MEMORY_PERSISTENT);

		//Mixed neg/pos
		for (ival = 0; ival < num; ++ival) {
			arr_32[ival] = (float32_t)random_range(low_range, high_range);
			arr_64[ival] = random_range(low_range, high_range);
		}

		sindex_32 = radixsort_sort(sort_32, arr_32, num);
		sindex_64 = radixsort_sort(sort_64, arr_64, num);

		if (num == 1) {
			EXPECT_EQ(sindex_32[0], 0);
			EXPECT_EQ(sindex_64[0], 0);
		}
		else for (ival = 1; ival < num; ++ival) {
				for (sval = 0; sval < ival; ++sval) {
					EXPECT_NE(sindex_32[sval], sindex_32[ival]);
					EXPECT_NE(sindex_64[sval], sindex_64[ival]);
				}
				EXPECT_LE(arr_32[ sindex_32[ival - 1] ], arr_32[ sindex_32[ival] ]);
				EXPECT_LE(arr_64[ sindex_64[ival - 1] ], arr_64[ sindex_64[ival] ]);
			}

		//Only neg
		for (ival = 0; ival < num; ++ival) {
			arr_32[ival] = (float32_t)random_range(low_range, -1.0f);
			arr_64[ival] = random_range(low_range, -1.0f);
		}

		sindex_32 = radixsort_sort(sort_32, arr_32, num);
		sindex_64 = radixsort_sort(sort_64, arr_64, num);

		if (num == 1) {
			EXPECT_EQ(sindex_32[0], 0);
			EXPECT_EQ(sindex_64[0], 0);
		}
		else for (ival = 1; ival < num; ++ival) {
				for (sval = 0; sval < ival; ++sval) {
					EXPECT_NE(sindex_32[sval], sindex_32[ival]);
					EXPECT_NE(sindex_64[sval], sindex_64[ival]);
				}
				EXPECT_LE(arr_32[ sindex_32[ival - 1] ], arr_32[ sindex_32[ival] ]);
				EXPECT_LE(arr_64[ sindex_64[ival - 1] ], arr_64[ sindex_64[ival] ]);
			}

		memory_deallocate(arr_32);
		memory_deallocate(arr_64);
	}

	for (; bits >= 1; --bits) {
		radixsort_index_t ival, sval;
		radixsort_index_t num = (radixsort_index_t)((1ULL << (uint64_t)bits) - 1);
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_32;
		const radixsort_index_t* FOUNDATION_RESTRICT sindex_64;

		arr_32 = memory_allocate(0, sizeof(float32_t) * num, 0, MEMORY_PERSISTENT);
		arr_64 = memory_allocate(0, sizeof(float64_t) * num, 0, MEMORY_PERSISTENT);

		//Mixed neg/pos
		for (ival = 0; ival < num; ++ival) {
			arr_32[ival] = (float32_t)random_range(low_range, high_range);
			arr_64[ival] = random_range(low_range, high_range);
		}

		sindex_32 = radixsort_sort(sort_32, arr_32, num);
		sindex_64 = radixsort_sort(sort_64, arr_64, num);

		if (num == 1) {
			EXPECT_EQ(sindex_32[0], 0);
			EXPECT_EQ(sindex_64[0], 0);
		}
		else for (ival = 1; ival < num; ++ival) {
				for (sval = 0; sval < ival; ++sval) {
					EXPECT_NE(sindex_32[sval], sindex_32[ival]);
					EXPECT_NE(sindex_64[sval], sindex_64[ival]);
				}
				EXPECT_LE(arr_32[ sindex_32[ival - 1] ], arr_32[ sindex_32[ival] ]);
				EXPECT_LE(arr_64[ sindex_64[ival - 1] ], arr_64[ sindex_64[ival] ]);
			}

		//Only neg
		for (ival = 0; ival < num; ++ival) {
			arr_32[ival] = (float32_t)random_range(low_range, -1.0f);
			arr_64[ival] = random_range(low_range, -1.0f);
		}

		sindex_32 = radixsort_sort(sort_32, arr_32, num);
		sindex_64 = radixsort_sort(sort_64, arr_64, num);

		if (num == 1) {
			EXPECT_EQ(sindex_32[0], 0);
			EXPECT_EQ(sindex_64[0], 0);
		}
		else for (ival = 1; ival < num; ++ival) {
				for (sval = 0; sval < ival; ++sval) {
					EXPECT_NE(sindex_32[sval], sindex_32[ival]);
					EXPECT_NE(sindex_64[sval], sindex_64[ival]);
				}
				EXPECT_LE(arr_32[ sindex_32[ival - 1] ], arr_32[ sindex_32[ival] ]);
				EXPECT_LE(arr_64[ sindex_64[ival - 1] ], arr_64[ sindex_64[ival] ]);
			}

		memory_deallocate(arr_32);
		memory_deallocate(arr_64);
	}

	radixsort_deallocate(sort_32);
	radixsort_deallocate(sort_64);

	return 0;
}

static void
test_radixsort_declare(void) {
	ADD_TEST(radixsort, allocation);
	ADD_TEST(radixsort, sort_int32);
	ADD_TEST(radixsort, sort_int64);
	ADD_TEST(radixsort, sort_real);
}

static test_suite_t test_radixsort_suite = {
	test_radixsort_application,
	test_radixsort_memory_system,
	test_radixsort_config,
	test_radixsort_declare,
	test_radixsort_initialize,
	test_radixsort_finalize,
	0
};

#if BUILD_MONOLITHIC

int
test_radixsort_run(void);

int
test_radixsort_run(void) {
	test_suite = test_radixsort_suite;
	return test_run_all();
}

#else

test_suite_t
test_suite_define(void);

test_suite_t
test_suite_define(void) {
	return test_radixsort_suite;
}

#endif
