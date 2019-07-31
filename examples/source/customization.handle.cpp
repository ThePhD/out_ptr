//  Copyright ⓒ 2018-2019 ThePhD.
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/out_ptr/ for documentation.


#include <phd/handle.hpp>

#include <ficapi/ficapi.hpp>

#include <phd/out_ptr.hpp>

#include <assert.hpp>

#include <type_traits>

namespace phd { namespace out_ptr {

	// this is the full out_ptr_t customization point
	// you can also specialize the inout_ptr_t for that template too
	template <typename T, typename D, typename Pointer, typename... Args>
	class inout_ptr_t<phd::handle<T, D>, Pointer, Args...> : std::tuple<Args...> {
	private:
		using Smart		 = phd::handle<T, D>;
		using source_pointer = pointer_of_or_t<Smart, Pointer>;
		using ArgsTuple	 = std::tuple<Args...>;
		using Base		 = ArgsTuple;

		Pointer* m_target_ptr;

	public:
		inout_ptr_t(Smart& s, Args... args) noexcept
		: Base(std::forward<Args>(args)...), m_target_ptr(static_cast<Pointer*>(static_cast<void*>(std::addressof(s.get())))) {
		}

		inout_ptr_t(inout_ptr_t&& right) noexcept
		: Base(std::move(right)), m_target_ptr(right.m_target_ptr) {
			right.m_target_ptr = nullptr;
		}
		inout_ptr_t& operator=(inout_ptr_t&& right) noexcept {
			Base::operator	=(std::move(right));
			this->m_target_ptr = right.m_target_ptr;
			return *this;
		}

		operator Pointer*() const noexcept {
			return const_cast<Pointer*>(this->m_target_ptr);
		}

		~inout_ptr_t() noexcept {
			static_assert(sizeof...(Args) < 1, "you cannot reset the deleter or pass more arguments for handle<T, Deleter>!: it only takes one argument!");
			reset();
		}

	private:
		void reset() {
			// internal pointer has already been written into,
			// we are fine
		}
	};
}} // namespace phd::out_ptr

int main() {
	phd::handle<int*, ficapi::int_deleter> p(nullptr);
	ficapi_re_create(phd::out_ptr::inout_ptr<void*>(p), ficapi_type::ficapi_type_int);
	int* rawp = static_cast<int*>(p.get());

	OUT_PTR_C_ASSERT(rawp != nullptr);
	OUT_PTR_C_ASSERT(*rawp == ficapi_get_dynamic_data());
}
