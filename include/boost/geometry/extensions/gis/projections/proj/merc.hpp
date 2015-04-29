#ifndef BOOST_GEOMETRY_PROJECTIONS_MERC_HPP
#define BOOST_GEOMETRY_PROJECTIONS_MERC_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2015 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Last updated version of proj: 4.9.1

// Original copyright notice:
 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_msfn.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_phi2.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_tsfn.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace merc{
            static const double EPS10 = 1.e-10;

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_merc_ellipsoid : public base_t_fi<base_merc_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;


                inline base_merc_ellipsoid(const Parameters& par)
                    : base_t_fi<base_merc_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    if (fabs(fabs(lp_lat) - HALFPI) <= EPS10) throw proj_exception();;
                    xy_x = this->m_par.k0 * lp_lon;
                    xy_y = - this->m_par.k0 * log(pj_tsfn(lp_lat, sin(lp_lat), this->m_par.e));
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    if ((lp_lat = pj_phi2(exp(- xy_y / this->m_par.k0), this->m_par.e)) == HUGE_VAL) throw proj_exception();;
                    lp_lon = xy_x / this->m_par.k0;
                }
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_merc_spheroid : public base_t_fi<base_merc_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;


                inline base_merc_spheroid(const Parameters& par)
                    : base_t_fi<base_merc_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    if (fabs(fabs(lp_lat) - HALFPI) <= EPS10) throw proj_exception();;
                    xy_x = this->m_par.k0 * lp_lon;
                    xy_y = this->m_par.k0 * log(tan(FORTPI + .5 * lp_lat));
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    lp_lat = HALFPI - 2. * atan(exp(-xy_y / this->m_par.k0));
                    lp_lon = xy_x / this->m_par.k0;
                }
            };

            // Mercator
            template <typename Parameters>
            void setup_merc(Parameters& par)
            {
                double phits=0.0;
                int is_phits;
                if( (is_phits = pj_param(par.params, "tlat_ts").i) ) {
                    phits = fabs(pj_param(par.params, "rlat_ts").f);
                    if (phits >= HALFPI) throw proj_exception(-24);
                }
                if (par.es) { /* ellipsoid */
                    if (is_phits)
                        par.k0 = pj_msfn(sin(phits), cos(phits), par.es);
                // par.inv = e_inverse;
                // par.fwd = e_forward;
                } else { /* sphere */
                    if (is_phits)
                        par.k0 = cos(phits);
                // par.inv = s_inverse;
                // par.fwd = s_forward;
                }
            }

        }} // namespace detail::merc
    #endif // doxygen

    /*!
        \brief Mercator projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Cylindrical
         - Spheroid
         - Ellipsoid
         - lat_ts=
        \par Example
        \image html ex_merc.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct merc_ellipsoid : public detail::merc::base_merc_ellipsoid<Geographic, Cartesian, Parameters>
    {
        inline merc_ellipsoid(const Parameters& par) : detail::merc::base_merc_ellipsoid<Geographic, Cartesian, Parameters>(par)
        {
            detail::merc::setup_merc(this->m_par);
        }
    };

    /*!
        \brief Mercator projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Cylindrical
         - Spheroid
         - Ellipsoid
         - lat_ts=
        \par Example
        \image html ex_merc.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct merc_spheroid : public detail::merc::base_merc_spheroid<Geographic, Cartesian, Parameters>
    {
        inline merc_spheroid(const Parameters& par) : detail::merc::base_merc_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::merc::setup_merc(this->m_par);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class merc_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    if (par.es)
                        return new base_v_fi<merc_ellipsoid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                    else
                        return new base_v_fi<merc_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void merc_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("merc", new merc_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_MERC_HPP

