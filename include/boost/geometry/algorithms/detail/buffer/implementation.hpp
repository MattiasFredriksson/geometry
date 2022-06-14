// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// This file was modified by Oracle on 2017-2022.
// Modifications copyright (c) 2017-2022 Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_DETAIL_BUFFER_IMPLEMENTATION_HPP
#define BOOST_GEOMETRY_ALGORITHMS_DETAIL_BUFFER_IMPLEMENTATION_HPP

#include <boost/range/value_type.hpp>

#include <boost/geometry/algorithms/detail/buffer/buffer_box.hpp>
#include <boost/geometry/algorithms/detail/buffer/buffer_inserter.hpp>
#include <boost/geometry/algorithms/detail/buffer/interface.hpp>
#include <boost/geometry/algorithms/envelope.hpp>
#include <boost/geometry/algorithms/is_empty.hpp>
#include <boost/geometry/arithmetic/arithmetic.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/strategies/buffer/cartesian.hpp>
#include <boost/geometry/strategies/buffer/geographic.hpp>
#include <boost/geometry/strategies/buffer/spherical.hpp>
#include <boost/geometry/util/math.hpp>

namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename BoxIn, typename BoxOut>
struct buffer_dc<BoxIn, BoxOut, box_tag, box_tag>
{
    template <typename Distance>
    static inline void apply(BoxIn const& box_in, BoxOut& box_out,
                             Distance const& distance, Distance const& )
    {
        detail::buffer::buffer_box(box_in, distance, box_out);
    }
};


template <typename Input, typename Output, typename TagIn>
struct buffer_all<Input, Output, TagIn, multi_polygon_tag>
{
    template
    <
        typename Output,
        typename DistanceStrategy,
        typename SideStrategy,
        typename JoinStrategy,
        typename EndStrategy,
        typename PointStrategy,
        typename Strategies
    >
    static inline void apply(Input const& geometry_in,
                             Output& geometry_out,
                             DistanceStrategy const& distance_strategy,
                             SideStrategy const& side_strategy,
                             JoinStrategy const& join_strategy,
                             EndStrategy const& end_strategy,
                             PointStrategy const& point_strategy,
                             Strategies const& strategies)
    {
        typedef typename boost::range_value<Output>::type polygon_type;

        typedef typename point_type<Input>::type point_type;
        typedef typename rescale_policy_type
            <
                point_type,
                typename geometry::cs_tag<point_type>::type
            >::type rescale_policy_type;

        if (geometry::is_empty(geometry_in))
        {
            // Then output geometry is kept empty as well
            return;
        }

        model::box<point_type> box;
        geometry::envelope(geometry_in, box);
        geometry::buffer(box, box, distance_strategy.max_distance(join_strategy, end_strategy));

        rescale_policy_type rescale_policy
                = boost::geometry::get_rescale_policy<rescale_policy_type>(
                    box, strategies);

        detail::buffer::buffer_inserter<polygon_type>(geometry_in,
                    range::back_inserter(geometry_out),
                    distance_strategy,
                    side_strategy,
                    join_strategy,
                    end_strategy,
                    point_strategy,
                    strategies,
                    rescale_policy);
    }
};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH

}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_ALGORITHMS_DETAIL_BUFFER_IMPLEMENTATION_HPP
