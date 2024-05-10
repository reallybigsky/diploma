#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/simple/Rectangle.hpp"



namespace fuzz_details {

struct Picture {
    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename simple::Picture_BASE<BOXED>::Builder {}
                .set_fields_mask(fields_mask)
                .set_r(r.toBuilder<true>());
    }

    uint32_t fields_mask;
    Rectangle r;
};

auto domainPicture()
{
    return fuzztest::StructOf<Picture>(fuzztest::InRange<uint32_t>(0, 7),
                                         domainRectangle());
}



template <bool INNER_BOXED>
struct ArrayPicture {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename simple::Picture_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, simple::Picture_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<Picture> arr;
};

template <bool INNER_BOXED>
auto domainArrayPicture()
{
    return fuzztest::StructOf<ArrayPicture<INNER_BOXED>>(fuzztest::VectorOf(domainPicture()).WithMaxSize(16));
}


}    // namespace fuzz_details
