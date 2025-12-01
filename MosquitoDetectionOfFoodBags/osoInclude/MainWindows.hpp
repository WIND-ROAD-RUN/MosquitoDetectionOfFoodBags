#pragma once

#include"oso_core.h"
#include <string>

namespace cdm {
    class MainWindowsConfig
    {
    public:
        MainWindowsConfig() = default;
        ~MainWindowsConfig() = default;

        MainWindowsConfig(const rw::oso::ObjectStoreAssembly& assembly);
        MainWindowsConfig(const MainWindowsConfig& obj);

        MainWindowsConfig& operator=(const MainWindowsConfig& obj);
        operator rw::oso::ObjectStoreAssembly() const;
        bool operator==(const MainWindowsConfig& obj) const;
        bool operator!=(const MainWindowsConfig& obj) const;

    public:
        int totalProductionVolume{ 0 };
        int totalDefectiveVolume{ 0 };
        double productionYield{ 0.0 };
        bool isDebug{ false };
        bool isDefect{ false };
        bool isSaveImg{ false };
        bool isshibiekuang{ true };
        bool iswenzi{ true };
        double baoguang{ 0.0 };
    };

    inline MainWindowsConfig::MainWindowsConfig(const rw::oso::ObjectStoreAssembly& assembly)
    {
        auto isAccountAssembly = assembly.getName();
        if (isAccountAssembly != "$class$MainWindowsConfig$")
        {
            throw std::runtime_error("Assembly is not $class$MainWindowsConfig$");
        }
        auto totalProductionVolumeItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$totalProductionVolume$"));
        if (!totalProductionVolumeItem) {
            throw std::runtime_error("$variable$totalProductionVolume is not found");
        }
        totalProductionVolume = totalProductionVolumeItem->getValueAsInt();
        auto totalDefectiveVolumeItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$totalDefectiveVolume$"));
        if (!totalDefectiveVolumeItem) {
            throw std::runtime_error("$variable$totalDefectiveVolume is not found");
        }
        totalDefectiveVolume = totalDefectiveVolumeItem->getValueAsInt();
        auto productionYieldItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$productionYield$"));
        if (!productionYieldItem) {
            throw std::runtime_error("$variable$productionYield is not found");
        }
        productionYield = productionYieldItem->getValueAsDouble();
        auto isDebugItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isDebug$"));
        if (!isDebugItem) {
            throw std::runtime_error("$variable$isDebug is not found");
        }
        isDebug = isDebugItem->getValueAsBool();
        auto isDefectItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isDefect$"));
        if (!isDefectItem) {
            throw std::runtime_error("$variable$isDefect is not found");
        }
        isDefect = isDefectItem->getValueAsBool();
        auto isSaveImgItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isSaveImg$"));
        if (!isSaveImgItem) {
            throw std::runtime_error("$variable$isSaveImg is not found");
        }
        isSaveImg = isSaveImgItem->getValueAsBool();
        auto isshibiekuangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isshibiekuang$"));
        if (!isshibiekuangItem) {
            throw std::runtime_error("$variable$isshibiekuang is not found");
        }
        isshibiekuang = isshibiekuangItem->getValueAsBool();
        auto iswenziItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$iswenzi$"));
        if (!iswenziItem) {
            throw std::runtime_error("$variable$iswenzi is not found");
        }
        iswenzi = iswenziItem->getValueAsBool();
        auto baoguangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$baoguang$"));
        if (!baoguangItem) {
            throw std::runtime_error("$variable$baoguang is not found");
        }
        baoguang = baoguangItem->getValueAsDouble();
    }

    inline MainWindowsConfig::MainWindowsConfig(const MainWindowsConfig& obj)
    {
        totalProductionVolume = obj.totalProductionVolume;
        totalDefectiveVolume = obj.totalDefectiveVolume;
        productionYield = obj.productionYield;
        isDebug = obj.isDebug;
        isDefect = obj.isDefect;
        isSaveImg = obj.isSaveImg;
        isshibiekuang = obj.isshibiekuang;
        iswenzi = obj.iswenzi;
        baoguang = obj.baoguang;
    }

    inline MainWindowsConfig& MainWindowsConfig::operator=(const MainWindowsConfig& obj)
    {
        if (this != &obj) {
            totalProductionVolume = obj.totalProductionVolume;
            totalDefectiveVolume = obj.totalDefectiveVolume;
            productionYield = obj.productionYield;
            isDebug = obj.isDebug;
            isDefect = obj.isDefect;
            isSaveImg = obj.isSaveImg;
            isshibiekuang = obj.isshibiekuang;
            iswenzi = obj.iswenzi;
            baoguang = obj.baoguang;
        }
        return *this;
    }

    inline MainWindowsConfig::operator rw::oso::ObjectStoreAssembly() const
    {
        rw::oso::ObjectStoreAssembly assembly;
        assembly.setName("$class$MainWindowsConfig$");
        auto totalProductionVolumeItem = std::make_shared<rw::oso::ObjectStoreItem>();
        totalProductionVolumeItem->setName("$variable$totalProductionVolume$");
        totalProductionVolumeItem->setValueFromInt(totalProductionVolume);
        assembly.addItem(totalProductionVolumeItem);
        auto totalDefectiveVolumeItem = std::make_shared<rw::oso::ObjectStoreItem>();
        totalDefectiveVolumeItem->setName("$variable$totalDefectiveVolume$");
        totalDefectiveVolumeItem->setValueFromInt(totalDefectiveVolume);
        assembly.addItem(totalDefectiveVolumeItem);
        auto productionYieldItem = std::make_shared<rw::oso::ObjectStoreItem>();
        productionYieldItem->setName("$variable$productionYield$");
        productionYieldItem->setValueFromDouble(productionYield);
        assembly.addItem(productionYieldItem);
        auto isDebugItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isDebugItem->setName("$variable$isDebug$");
        isDebugItem->setValueFromBool(isDebug);
        assembly.addItem(isDebugItem);
        auto isDefectItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isDefectItem->setName("$variable$isDefect$");
        isDefectItem->setValueFromBool(isDefect);
        assembly.addItem(isDefectItem);
        auto isSaveImgItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isSaveImgItem->setName("$variable$isSaveImg$");
        isSaveImgItem->setValueFromBool(isSaveImg);
        assembly.addItem(isSaveImgItem);
        auto isshibiekuangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isshibiekuangItem->setName("$variable$isshibiekuang$");
        isshibiekuangItem->setValueFromBool(isshibiekuang);
        assembly.addItem(isshibiekuangItem);
        auto iswenziItem = std::make_shared<rw::oso::ObjectStoreItem>();
        iswenziItem->setName("$variable$iswenzi$");
        iswenziItem->setValueFromBool(iswenzi);
        assembly.addItem(iswenziItem);
        auto baoguangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        baoguangItem->setName("$variable$baoguang$");
        baoguangItem->setValueFromDouble(baoguang);
        assembly.addItem(baoguangItem);
        return assembly;
    }

    inline bool MainWindowsConfig::operator==(const MainWindowsConfig& obj) const
    {
        return totalProductionVolume == obj.totalProductionVolume && totalDefectiveVolume == obj.totalDefectiveVolume && productionYield == obj.productionYield && isDebug == obj.isDebug && isDefect == obj.isDefect && isSaveImg == obj.isSaveImg && isshibiekuang == obj.isshibiekuang && iswenzi == obj.iswenzi && baoguang == obj.baoguang;
    }

    inline bool MainWindowsConfig::operator!=(const MainWindowsConfig& obj) const
    {
        return !(*this == obj);
    }

}

