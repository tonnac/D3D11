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

#include "../ILightingUnits.h"

struct NotifyInfo;

/*! \file
 * \warning This API is unsupported and provided for experiment only.  It should not be used for production.
 * This API will change or disappear in a future release.
 */

namespace MaxSDK
{;

/*!
 * \brief Qt Max spin box for lighting units.
 * \warning This API is unsupported and provided for experiment only.  It should not be used for production.
 * This API will change or disappear in a future release.
*/

// Implements a spin box that displays lighting units
class MaxQTExport QMaxLightingUnitSpinBox : public QMaxFloatSpinBox
{
    Q_OBJECT

    // The type of quantity being represented by this spin box
    Q_PROPERTY(QuantityType quantityType READ quantityType WRITE setQuantityType)
    // The unit system in which the internal value is represented (i.e. what's stored in the 'value' property).
    // The spin box will convert from this system to the display system when displaying the value.
    Q_PROPERTY(UnitSystem internalUnitSystem READ internalUnitSystem WRITE setInternalUnitSystem)

public:

    // The type of quantity being represented by this spin box
    enum QuantityType
    {
        Luminance,
        Illuminance
    };
    typedef ILightingUnits::LightingSystems UnitSystem;

    explicit QMaxLightingUnitSpinBox(QWidget *parent = 0);
    ~QMaxLightingUnitSpinBox();

    QuantityType quantityType() const;
    void setQuantityType(const QuantityType val);
    
    UnitSystem internalUnitSystem() const;
    void setInternalUnitSystem(const UnitSystem val);

signals:

    void quantityTypeChanged(QuantityType);
    void internalUnitSystemChanged(UnitSystem);

protected:

    // Updates the unit suffix being used - to be called when the display unit system changes
    void update_unit_suffix();

    // -- from QDoubleSpinBox
    // Override the text<->value conversions to display in desired units
    virtual QString textFromValue(double val) const override;
    virtual double valueFromText(const QString &text) const override;

private:

    // Notification callback for 3ds max notifications
    static void notification_proc(void *param, NotifyInfo* info);

private:

    QuantityType m_quantityType;
    UnitSystem m_internalUnitSystem;
};


};
