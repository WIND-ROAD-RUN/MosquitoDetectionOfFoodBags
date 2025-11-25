#pragma once

#include"oso_core.h"
#include <string>

namespace cdm {
    class SetConfig
    {
    public:
        SetConfig() = default;
        ~SetConfig() = default;

        SetConfig(const rw::oso::ObjectStoreAssembly& assembly);
        SetConfig(const SetConfig& obj);

        SetConfig& operator=(const SetConfig& obj);
        operator rw::oso::ObjectStoreAssembly() const;
        bool operator==(const SetConfig& obj) const;
        bool operator!=(const SetConfig& obj) const;

    public:
        double tifeichixushijian1{ 0 };
        double tifeijuli1{ 0 };
        double shangXianWei1{ 0 };
        double xiaXianWei1{ 0 };
        double zuoXianWei1{ 0 };
        double youXianWei1{ 0 };
        double baoguang1{ 0 };
        double zengyi1{ 0 };
        double xiangSuDangLiang1{ 0 };
        bool yundongkongzhiqichonglian{ false };
        bool debugMode{ false };
        int tifeixinhaoOUT{ 0 };
        int xiangjichufa1OUT{ 0 };
        int xiangjichufa2OUT{ 0 };
        double xiangjichufachangdu{ 0 };
        double meizhuanmaichongshu{ 0 };
        double shedingzhouchang{ 0 };
    };

    inline SetConfig::SetConfig(const rw::oso::ObjectStoreAssembly& assembly)
    {
        auto isAccountAssembly = assembly.getName();
        if (isAccountAssembly != "$class$SetConfig$")
        {
            throw std::runtime_error("Assembly is not $class$SetConfig$");
        }
        auto tifeichixushijian1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tifeichixushijian1$"));
        if (!tifeichixushijian1Item) {
            throw std::runtime_error("$variable$tifeichixushijian1 is not found");
        }
        tifeichixushijian1 = tifeichixushijian1Item->getValueAsDouble();
        auto tifeijuli1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tifeijuli1$"));
        if (!tifeijuli1Item) {
            throw std::runtime_error("$variable$tifeijuli1 is not found");
        }
        tifeijuli1 = tifeijuli1Item->getValueAsDouble();
        auto shangXianWei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$shangXianWei1$"));
        if (!shangXianWei1Item) {
            throw std::runtime_error("$variable$shangXianWei1 is not found");
        }
        shangXianWei1 = shangXianWei1Item->getValueAsDouble();
        auto xiaXianWei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiaXianWei1$"));
        if (!xiaXianWei1Item) {
            throw std::runtime_error("$variable$xiaXianWei1 is not found");
        }
        xiaXianWei1 = xiaXianWei1Item->getValueAsDouble();
        auto zuoXianWei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zuoXianWei1$"));
        if (!zuoXianWei1Item) {
            throw std::runtime_error("$variable$zuoXianWei1 is not found");
        }
        zuoXianWei1 = zuoXianWei1Item->getValueAsDouble();
        auto youXianWei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$youXianWei1$"));
        if (!youXianWei1Item) {
            throw std::runtime_error("$variable$youXianWei1 is not found");
        }
        youXianWei1 = youXianWei1Item->getValueAsDouble();
        auto baoguang1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$baoguang1$"));
        if (!baoguang1Item) {
            throw std::runtime_error("$variable$baoguang1 is not found");
        }
        baoguang1 = baoguang1Item->getValueAsDouble();
        auto zengyi1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zengyi1$"));
        if (!zengyi1Item) {
            throw std::runtime_error("$variable$zengyi1 is not found");
        }
        zengyi1 = zengyi1Item->getValueAsDouble();
        auto xiangSuDangLiang1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiangSuDangLiang1$"));
        if (!xiangSuDangLiang1Item) {
            throw std::runtime_error("$variable$xiangSuDangLiang1 is not found");
        }
        xiangSuDangLiang1 = xiangSuDangLiang1Item->getValueAsDouble();
        auto yundongkongzhiqichonglianItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$yundongkongzhiqichonglian$"));
        if (!yundongkongzhiqichonglianItem) {
            throw std::runtime_error("$variable$yundongkongzhiqichonglian is not found");
        }
        yundongkongzhiqichonglian = yundongkongzhiqichonglianItem->getValueAsBool();
        auto debugModeItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$debugMode$"));
        if (!debugModeItem) {
            throw std::runtime_error("$variable$debugMode is not found");
        }
        debugMode = debugModeItem->getValueAsBool();
        auto tifeixinhaoOUTItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tifeixinhaoOUT$"));
        if (!tifeixinhaoOUTItem) {
            throw std::runtime_error("$variable$tifeixinhaoOUT is not found");
        }
        tifeixinhaoOUT = tifeixinhaoOUTItem->getValueAsInt();
        auto xiangjichufa1OUTItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiangjichufa1OUT$"));
        if (!xiangjichufa1OUTItem) {
            throw std::runtime_error("$variable$xiangjichufa1OUT is not found");
        }
        xiangjichufa1OUT = xiangjichufa1OUTItem->getValueAsInt();
        auto xiangjichufa2OUTItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiangjichufa2OUT$"));
        if (!xiangjichufa2OUTItem) {
            throw std::runtime_error("$variable$xiangjichufa2OUT is not found");
        }
        xiangjichufa2OUT = xiangjichufa2OUTItem->getValueAsInt();
        auto xiangjichufachangduItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiangjichufachangdu$"));
        if (!xiangjichufachangduItem) {
            throw std::runtime_error("$variable$xiangjichufachangdu is not found");
        }
        xiangjichufachangdu = xiangjichufachangduItem->getValueAsDouble();
        auto meizhuanmaichongshuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$meizhuanmaichongshu$"));
        if (!meizhuanmaichongshuItem) {
            throw std::runtime_error("$variable$meizhuanmaichongshu is not found");
        }
        meizhuanmaichongshu = meizhuanmaichongshuItem->getValueAsDouble();
        auto shedingzhouchangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$shedingzhouchang$"));
        if (!shedingzhouchangItem) {
            throw std::runtime_error("$variable$shedingzhouchang is not found");
        }
        shedingzhouchang = shedingzhouchangItem->getValueAsDouble();
    }

    inline SetConfig::SetConfig(const SetConfig& obj)
    {
        tifeichixushijian1 = obj.tifeichixushijian1;
        tifeijuli1 = obj.tifeijuli1;
        shangXianWei1 = obj.shangXianWei1;
        xiaXianWei1 = obj.xiaXianWei1;
        zuoXianWei1 = obj.zuoXianWei1;
        youXianWei1 = obj.youXianWei1;
        baoguang1 = obj.baoguang1;
        zengyi1 = obj.zengyi1;
        xiangSuDangLiang1 = obj.xiangSuDangLiang1;
        yundongkongzhiqichonglian = obj.yundongkongzhiqichonglian;
        debugMode = obj.debugMode;
        tifeixinhaoOUT = obj.tifeixinhaoOUT;
        xiangjichufa1OUT = obj.xiangjichufa1OUT;
        xiangjichufa2OUT = obj.xiangjichufa2OUT;
        xiangjichufachangdu = obj.xiangjichufachangdu;
        meizhuanmaichongshu = obj.meizhuanmaichongshu;
        shedingzhouchang = obj.shedingzhouchang;
    }

    inline SetConfig& SetConfig::operator=(const SetConfig& obj)
    {
        if (this != &obj) {
            tifeichixushijian1 = obj.tifeichixushijian1;
            tifeijuli1 = obj.tifeijuli1;
            shangXianWei1 = obj.shangXianWei1;
            xiaXianWei1 = obj.xiaXianWei1;
            zuoXianWei1 = obj.zuoXianWei1;
            youXianWei1 = obj.youXianWei1;
            baoguang1 = obj.baoguang1;
            zengyi1 = obj.zengyi1;
            xiangSuDangLiang1 = obj.xiangSuDangLiang1;
            yundongkongzhiqichonglian = obj.yundongkongzhiqichonglian;
            debugMode = obj.debugMode;
            tifeixinhaoOUT = obj.tifeixinhaoOUT;
            xiangjichufa1OUT = obj.xiangjichufa1OUT;
            xiangjichufa2OUT = obj.xiangjichufa2OUT;
            xiangjichufachangdu = obj.xiangjichufachangdu;
            meizhuanmaichongshu = obj.meizhuanmaichongshu;
            shedingzhouchang = obj.shedingzhouchang;
        }
        return *this;
    }

    inline SetConfig::operator rw::oso::ObjectStoreAssembly() const
    {
        rw::oso::ObjectStoreAssembly assembly;
        assembly.setName("$class$SetConfig$");
        auto tifeichixushijian1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        tifeichixushijian1Item->setName("$variable$tifeichixushijian1$");
        tifeichixushijian1Item->setValueFromDouble(tifeichixushijian1);
        assembly.addItem(tifeichixushijian1Item);
        auto tifeijuli1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        tifeijuli1Item->setName("$variable$tifeijuli1$");
        tifeijuli1Item->setValueFromDouble(tifeijuli1);
        assembly.addItem(tifeijuli1Item);
        auto shangXianWei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        shangXianWei1Item->setName("$variable$shangXianWei1$");
        shangXianWei1Item->setValueFromDouble(shangXianWei1);
        assembly.addItem(shangXianWei1Item);
        auto xiaXianWei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        xiaXianWei1Item->setName("$variable$xiaXianWei1$");
        xiaXianWei1Item->setValueFromDouble(xiaXianWei1);
        assembly.addItem(xiaXianWei1Item);
        auto zuoXianWei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        zuoXianWei1Item->setName("$variable$zuoXianWei1$");
        zuoXianWei1Item->setValueFromDouble(zuoXianWei1);
        assembly.addItem(zuoXianWei1Item);
        auto youXianWei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        youXianWei1Item->setName("$variable$youXianWei1$");
        youXianWei1Item->setValueFromDouble(youXianWei1);
        assembly.addItem(youXianWei1Item);
        auto baoguang1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        baoguang1Item->setName("$variable$baoguang1$");
        baoguang1Item->setValueFromDouble(baoguang1);
        assembly.addItem(baoguang1Item);
        auto zengyi1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        zengyi1Item->setName("$variable$zengyi1$");
        zengyi1Item->setValueFromDouble(zengyi1);
        assembly.addItem(zengyi1Item);
        auto xiangSuDangLiang1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        xiangSuDangLiang1Item->setName("$variable$xiangSuDangLiang1$");
        xiangSuDangLiang1Item->setValueFromDouble(xiangSuDangLiang1);
        assembly.addItem(xiangSuDangLiang1Item);
        auto yundongkongzhiqichonglianItem = std::make_shared<rw::oso::ObjectStoreItem>();
        yundongkongzhiqichonglianItem->setName("$variable$yundongkongzhiqichonglian$");
        yundongkongzhiqichonglianItem->setValueFromBool(yundongkongzhiqichonglian);
        assembly.addItem(yundongkongzhiqichonglianItem);
        auto debugModeItem = std::make_shared<rw::oso::ObjectStoreItem>();
        debugModeItem->setName("$variable$debugMode$");
        debugModeItem->setValueFromBool(debugMode);
        assembly.addItem(debugModeItem);
        auto tifeixinhaoOUTItem = std::make_shared<rw::oso::ObjectStoreItem>();
        tifeixinhaoOUTItem->setName("$variable$tifeixinhaoOUT$");
        tifeixinhaoOUTItem->setValueFromInt(tifeixinhaoOUT);
        assembly.addItem(tifeixinhaoOUTItem);
        auto xiangjichufa1OUTItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiangjichufa1OUTItem->setName("$variable$xiangjichufa1OUT$");
        xiangjichufa1OUTItem->setValueFromInt(xiangjichufa1OUT);
        assembly.addItem(xiangjichufa1OUTItem);
        auto xiangjichufa2OUTItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiangjichufa2OUTItem->setName("$variable$xiangjichufa2OUT$");
        xiangjichufa2OUTItem->setValueFromInt(xiangjichufa2OUT);
        assembly.addItem(xiangjichufa2OUTItem);
        auto xiangjichufachangduItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiangjichufachangduItem->setName("$variable$xiangjichufachangdu$");
        xiangjichufachangduItem->setValueFromDouble(xiangjichufachangdu);
        assembly.addItem(xiangjichufachangduItem);
        auto meizhuanmaichongshuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        meizhuanmaichongshuItem->setName("$variable$meizhuanmaichongshu$");
        meizhuanmaichongshuItem->setValueFromDouble(meizhuanmaichongshu);
        assembly.addItem(meizhuanmaichongshuItem);
        auto shedingzhouchangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        shedingzhouchangItem->setName("$variable$shedingzhouchang$");
        shedingzhouchangItem->setValueFromDouble(shedingzhouchang);
        assembly.addItem(shedingzhouchangItem);
        return assembly;
    }

    inline bool SetConfig::operator==(const SetConfig& obj) const
    {
        return tifeichixushijian1 == obj.tifeichixushijian1 && tifeijuli1 == obj.tifeijuli1 && shangXianWei1 == obj.shangXianWei1 && xiaXianWei1 == obj.xiaXianWei1 && zuoXianWei1 == obj.zuoXianWei1 && youXianWei1 == obj.youXianWei1 && baoguang1 == obj.baoguang1 && zengyi1 == obj.zengyi1 && xiangSuDangLiang1 == obj.xiangSuDangLiang1 && yundongkongzhiqichonglian == obj.yundongkongzhiqichonglian && debugMode == obj.debugMode && tifeixinhaoOUT == obj.tifeixinhaoOUT && xiangjichufa1OUT == obj.xiangjichufa1OUT && xiangjichufa2OUT == obj.xiangjichufa2OUT && xiangjichufachangdu == obj.xiangjichufachangdu && meizhuanmaichongshu == obj.meizhuanmaichongshu && shedingzhouchang == obj.shedingzhouchang;
    }

    inline bool SetConfig::operator!=(const SetConfig& obj) const
    {
        return !(*this == obj);
    }

}

