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

#include "QMaxFloatSpinBox.h"

/*! \file
 * \warning This API is unsupported and provided for experiment only.  It should not be used for production.
 * This API will change or disappear in a future release.
 */

namespace MaxSDK
{;

// This classes specializes QDoubleSpinBox to display world units (meters, etc.) in the same manner as the legacy 3ds Max spinner.
/*!
 * \brief Qt Max spin box for world units.
 * \warning This API is unsupported and provided for experiment only.  It should not be used for production.
 * This API will change or disappear in a future release.
*/
class MaxQTExport QMaxWorldSpinBox : public QMaxFloatSpinBox
{
public:

    explicit QMaxWorldSpinBox(QWidget *parent = 0);
    ~QMaxWorldSpinBox();

protected:

    // -- from QDoubleSpinBox
    // Override the text<->value conversions to display units correctly
    virtual QString textFromValue(double val) const override;
    virtual double valueFromText(const QString &text) const override;
    virtual QValidator::State validate(QString &input, int &pos) const override;
    virtual void fixup(QString &str) const override;

private:

    // Keeps track of last value which was converted into text. This is used to revert to the previous value when an invalid input is performed.
    mutable double m_last_converted_value;
};


};
