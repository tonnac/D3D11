//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2015 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "QtMax.h"
#include <QtGui/QSpinBox.h>

/*! \file
 * \warning This API is unsupported and provided for experiment only.  It should not be used for production.
 * This API will change or disappear in a future release.
 */

namespace MaxSDK
{;

// This classes specializes QDoubleSpinBox to implement behaviour specific to 3ds Max, emulating some behaviour of ISpinnerControl.
/*!
 * \brief Qt Max spin box for float values.
 * \warning This API is unsupported and provided for experiment only.  It should not be used for production.
 * This API will change or disappear in a future release.
*/
class MaxQTExport QMaxFloatSpinBox : public QDoubleSpinBox
{
public:

    // Initializes using the default spinner precision set in 3ds max
    explicit QMaxFloatSpinBox(QWidget* parent = nullptr);
    ~QMaxFloatSpinBox();

protected:

private:

};


};
