//  Copyright ⓒ 2018-2019 ThePhD.
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See https://github.com/ThePhD/out_ptr/blob/master/docs/out_ptr.adoc for documentation.

#include <benchmarks/statistics.hpp>

#include <benchmark/benchmark.h>

#define PHD_OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR 1

#include <benchmarks/out_ptr/friendly_unique_ptr.hpp>
#include <phd/out_ptr/inout_ptr.hpp>
#include <benchmarks/out_ptr/friendly_inout_ptr.hpp>

#include <ficapi/ficapi.hpp>

#include <memory>
#include <cstdlib>

static void c_code_local_inout_ptr(benchmark::State& state) {
	int64_t x = 0;
	for (auto _ : state) {
		(void)_;
		ficapi_opaque_handle p = NULL;
		ficapi_handle_no_alloc_re_create(&p);
		x += ficapi_handle_get_data(p);
		ficapi_handle_no_alloc_delete(p);
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(c_code_local_inout_ptr)

	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

static void manual_local_inout_ptr(benchmark::State& state) {
	int64_t x = 0;
	for (auto _ : state) {
		(void)_;
		std::unique_ptr<ficapi::opaque, ficapi::handle_no_alloc_deleter> p(nullptr);
		ficapi_opaque_handle temp_p = NULL;
		ficapi_handle_no_alloc_re_create(&temp_p);
		p.reset(temp_p);
		x += ficapi_handle_get_data(p.get());
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(manual_local_inout_ptr)

	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

static void simple_local_inout_ptr(benchmark::State& state) {
	int64_t x = 0;
	for (auto _ : state) {
		(void)_;
		std::unique_ptr<ficapi::opaque, ficapi::handle_no_alloc_deleter> p(nullptr);
		ficapi_handle_no_alloc_re_create(phd::out_ptr::detail::simple_inout_ptr(p));
		x += ficapi_handle_get_data(p.get());
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(simple_local_inout_ptr)

	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

static void clever_local_inout_ptr(benchmark::State& state) {
	int64_t x = 0;
	for (auto _ : state) {
		(void)_;
		std::unique_ptr<ficapi::opaque, ficapi::handle_no_alloc_deleter> p(nullptr);
		ficapi_handle_no_alloc_re_create(phd::out_ptr::detail::clever_inout_ptr(p));
		x += ficapi_handle_get_data(p.get());
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(clever_local_inout_ptr)

	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

#if defined(PHD_OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR) && PHD_OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR != 0

static void friendly_local_inout_ptr(benchmark::State& state) {
	int64_t x = 0;
	for (auto _ : state) {
		(void)_;
		std::friendly_unique_ptr<ficapi::opaque, ficapi::handle_no_alloc_deleter> p(nullptr);
		ficapi_handle_no_alloc_re_create(phd::out_ptr::friendly_inout_ptr(p));
		x += ficapi_handle_get_data(p.get());
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(friendly_local_inout_ptr)

	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

#endif // OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR