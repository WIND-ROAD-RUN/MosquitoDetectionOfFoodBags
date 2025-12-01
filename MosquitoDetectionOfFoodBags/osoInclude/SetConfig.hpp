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
        double tifeichixushijian{ 0 };
        double tifeijuli{ 0 };
        double shangXianWei{ 0 };
        double xiaXianWei{ 0 };
        double zuoXianWei{ 0 };
        double youXianWei{ 0 };
        double baoguang{ 0 };
        double zengyi{ 0 };
        double houFenPin{ 0 };
        double chengFaQi{ 0 };
        double xiangSuDangLiang{ 0 };
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
        auto tifeichixushijianItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tifeichixushijian$"));
        if (!tifeichixushijianItem) {
            throw std::runtime_error("$variable$tifeichixushijian is not found");
        }
        tifeichixushijian = tifeichixushijianItem->getValueAsDouble();
        auto tifeijuliItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tifeijuli$"));
        if (!tifeijuliItem) {
            throw std::runtime_error("$variable$tifeijuli is not found");
        }
        tifeijuli = tifeijuliItem->getValueAsDouble();
        auto shangXianWeiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$shangXianWei$"));
        if (!shangXianWeiItem) {
            throw std::runtime_error("$variable$shangXianWei is not found");
        }
        shangXianWei = shangXianWeiItem->getValueAsDouble();
        auto xiaXianWeiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiaXianWei$"));
        if (!xiaXianWeiItem) {
            throw std::runtime_error("$variable$xiaXianWei is not found");
        }
        xiaXianWei = xiaXianWeiItem->getValueAsDouble();
        auto zuoXianWeiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zuoXianWei$"));
        if (!zuoXianWeiItem) {
            throw std::runtime_error("$variable$zuoXianWei is not found");
        }
        zuoXianWei = zuoXianWeiItem->getValueAsDouble();
        auto youXianWeiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$youXianWei$"));
        if (!youXianWeiItem) {
            throw std::runtime_error("$variable$youXianWei is not found");
        }
        youXianWei = youXianWeiItem->getValueAsDouble();
        auto baoguangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$baoguang$"));
        if (!baoguangItem) {
            throw std::runtime_error("$variable$baoguang is not found");
        }
        baoguang = baoguangItem->getValueAsDouble();
        auto zengyiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zengyi$"));
        if (!zengyiItem) {
            throw std::runtime_error("$variable$zengyi is not found");
        }
        zengyi = zengyiItem->getValueAsDouble();
        auto houFenPinItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$houFenPin$"));
        if (!houFenPinItem) {
            throw std::runtime_error("$variable$houFenPin is not found");
        }
        houFenPin = houFenPinItem->getValueAsDouble();
        auto chengFaQiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$chengFaQi$"));
        if (!chengFaQiItem) {
            throw std::runtime_error("$variable$chengFaQi is not found");
        }
        chengFaQi = chengFaQiItem->getValueAsDouble();
        auto xiangSuDangLiangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiangSuDangLiang$"));
        if (!xiangSuDangLiangItem) {
            throw std::runtime_error("$variable$xiangSuDangLiang is not found");
        }
        xiangSuDangLiang = xiangSuDangLiangItem->getValueAsDouble();
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
        tifeichixushijian = obj.tifeichixushijian;
        tifeijuli = obj.tifeijuli;
        shangXianWei = obj.shangXianWei;
        xiaXianWei = obj.xiaXianWei;
        zuoXianWei = obj.zuoXianWei;
        youXianWei = obj.youXianWei;
        baoguang = obj.baoguang;
        zengyi = obj.zengyi;
        houFenPin = obj.houFenPin;
        chengFaQi = obj.chengFaQi;
        xiangSuDangLiang = obj.xiangSuDangLiang;
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
            tifeichixushijian = obj.tifeichixushijian;
            tifeijuli = obj.tifeijuli;
            shangXianWei = obj.shangXianWei;
            xiaXianWei = obj.xiaXianWei;
            zuoXianWei = obj.zuoXianWei;
            youXianWei = obj.youXianWei;
            baoguang = obj.baoguang;
            zengyi = obj.zengyi;
            houFenPin = obj.houFenPin;
            chengFaQi = obj.chengFaQi;
            xiangSuDangLiang = obj.xiangSuDangLiang;
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
        auto tifeichixushijianItem = std::make_shared<rw::oso::ObjectStoreItem>();
        tifeichixushijianItem->setName("$variable$tifeichixushijian$");
        tifeichixushijianItem->setValueFromDouble(tifeichixushijian);
        assembly.addItem(tifeichixushijianItem);
        auto tifeijuliItem = std::make_shared<rw::oso::ObjectStoreItem>();
        tifeijuliItem->setName("$variable$tifeijuli$");
        tifeijuliItem->setValueFromDouble(tifeijuli);
        assembly.addItem(tifeijuliItem);
        auto shangXianWeiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        shangXianWeiItem->setName("$variable$shangXianWei$");
        shangXianWeiItem->setValueFromDouble(shangXianWei);
        assembly.addItem(shangXianWeiItem);
        auto xiaXianWeiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiaXianWeiItem->setName("$variable$xiaXianWei$");
        xiaXianWeiItem->setValueFromDouble(xiaXianWei);
        assembly.addItem(xiaXianWeiItem);
        auto zuoXianWeiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zuoXianWeiItem->setName("$variable$zuoXianWei$");
        zuoXianWeiItem->setValueFromDouble(zuoXianWei);
        assembly.addItem(zuoXianWeiItem);
        auto youXianWeiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        youXianWeiItem->setName("$variable$youXianWei$");
        youXianWeiItem->setValueFromDouble(youXianWei);
        assembly.addItem(youXianWeiItem);
        auto baoguangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        baoguangItem->setName("$variable$baoguang$");
        baoguangItem->setValueFromDouble(baoguang);
        assembly.addItem(baoguangItem);
        auto zengyiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zengyiItem->setName("$variable$zengyi$");
        zengyiItem->setValueFromDouble(zengyi);
        assembly.addItem(zengyiItem);
        auto houFenPinItem = std::make_shared<rw::oso::ObjectStoreItem>();
        houFenPinItem->setName("$variable$houFenPin$");
        houFenPinItem->setValueFromDouble(houFenPin);
        assembly.addItem(houFenPinItem);
        auto chengFaQiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        chengFaQiItem->setName("$variable$chengFaQi$");
        chengFaQiItem->setValueFromDouble(chengFaQi);
        assembly.addItem(chengFaQiItem);
        auto xiangSuDangLiangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiangSuDangLiangItem->setName("$variable$xiangSuDangLiang$");
        xiangSuDangLiangItem->setValueFromDouble(xiangSuDangLiang);
        assembly.addItem(xiangSuDangLiangItem);
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
        return tifeichixushijian == obj.tifeichixushijian && tifeijuli == obj.tifeijuli && shangXianWei == obj.shangXianWei && xiaXianWei == obj.xiaXianWei && zuoXianWei == obj.zuoXianWei && youXianWei == obj.youXianWei && baoguang == obj.baoguang && zengyi == obj.zengyi && houFenPin == obj.houFenPin && chengFaQi == obj.chengFaQi && xiangSuDangLiang == obj.xiangSuDangLiang && yundongkongzhiqichonglian == obj.yundongkongzhiqichonglian && debugMode == obj.debugMode && tifeixinhaoOUT == obj.tifeixinhaoOUT && xiangjichufa1OUT == obj.xiangjichufa1OUT && xiangjichufa2OUT == obj.xiangjichufa2OUT && xiangjichufachangdu == obj.xiangjichufachangdu && meizhuanmaichongshu == obj.meizhuanmaichongshu && shedingzhouchang == obj.shedingzhouchang;
    }

    inline bool SetConfig::operator!=(const SetConfig& obj) const
    {
        return !(*this == obj);
    }

}

