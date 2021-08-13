#pragma once

#include <ode/ode.h>
#include <ode/odecpp.h>
#include "../math/MatrixUtils.h"

Matrix getRotPos(const dGeom&);
Matrix getRotPos(const dBody&);
