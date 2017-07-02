#pragma once

#include <boost/any.hpp>

#define POLY_VISITOR_PURE_VISITABLE(BASE_VISITOR) \
using poly_visitor_base_visitor = BASE_VISITOR;\
virtual boost::any accept(BASE_VISITOR&) = 0;\
virtual boost::any accept(BASE_VISITOR&) const = 0;

#define POLY_VISITOR_VISITABLE(BASE_VISITOR)\
using poly_visitor_base_visitor = BASE_VISITOR;\
boost::any accept(BASE_VISITOR& visitor) override\
{ return visitor.visit(*this); }\
boost::any accept(BASE_VISITOR& visitor) const override\
{ return visitor.visit(*this); }
