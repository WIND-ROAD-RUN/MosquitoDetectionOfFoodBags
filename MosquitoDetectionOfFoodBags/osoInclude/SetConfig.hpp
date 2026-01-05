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
        double shangxiasuojin{ 0 };
        double zuoXianWei{ 0 };
        double youXianWei{ 0 };
        double zengyi{ 0 };
        double houFenPin{ 0 };
        double chengFaQi{ 0 };
        double xiangSuDangLiang{ 0 };
        bool yundongkongzhiqichonglian{ false };
        bool debugMode{ false };
        int jiaodaitiewanIN{ 0 };
        int tifeixinhaoOUT{ 0 };
        int baojingOUT{ 0 };
        int lvdengOUT{ 0 };
        int hongdengOUT{ 0 };
        double xiangjichufachangdu{ 0 };
        double meizhuanmaichongshu{ 0 };
        double shedingzhouchang{ 0 };
        int tifeijishu{ 0 };
        int liangpinjishu{ 0 };
        double baojingchixushijian{ 0 };
        double wenchongzuidahuiduchazhi{ 0 };
        double wenchongzuixiaohuiduchazhi{ 0 };
        double wenchongzuidamianji{ 0 };
        double wenchongzuixiaomianji{ 0 };
        double maofazuidahuiduchazhi{ 0 };
        double maofazuixiaohuiduchazhi{ 0 };
        double maofazuidamianji{ 0 };
        double maofazuixiaomianji{ 0 };
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
        auto shangxiasuojinItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$shangxiasuojin$"));
        if (!shangxiasuojinItem) {
            throw std::runtime_error("$variable$shangxiasuojin is not found");
        }
        shangxiasuojin = shangxiasuojinItem->getValueAsDouble();
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
        auto jiaodaitiewanINItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$jiaodaitiewanIN$"));
        if (!jiaodaitiewanINItem) {
            throw std::runtime_error("$variable$jiaodaitiewanIN is not found");
        }
        jiaodaitiewanIN = jiaodaitiewanINItem->getValueAsInt();
        auto tifeixinhaoOUTItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tifeixinhaoOUT$"));
        if (!tifeixinhaoOUTItem) {
            throw std::runtime_error("$variable$tifeixinhaoOUT is not found");
        }
        tifeixinhaoOUT = tifeixinhaoOUTItem->getValueAsInt();
        auto baojingOUTItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$baojingOUT$"));
        if (!baojingOUTItem) {
            throw std::runtime_error("$variable$baojingOUT is not found");
        }
        baojingOUT = baojingOUTItem->getValueAsInt();
        auto lvdengOUTItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$lvdengOUT$"));
        if (!lvdengOUTItem) {
            throw std::runtime_error("$variable$lvdengOUT is not found");
        }
        lvdengOUT = lvdengOUTItem->getValueAsInt();
        auto hongdengOUTItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$hongdengOUT$"));
        if (!hongdengOUTItem) {
            throw std::runtime_error("$variable$hongdengOUT is not found");
        }
        hongdengOUT = hongdengOUTItem->getValueAsInt();
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
        auto tifeijishuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tifeijishu$"));
        if (!tifeijishuItem) {
            throw std::runtime_error("$variable$tifeijishu is not found");
        }
        tifeijishu = tifeijishuItem->getValueAsInt();
        auto liangpinjishuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$liangpinjishu$"));
        if (!liangpinjishuItem) {
            throw std::runtime_error("$variable$liangpinjishu is not found");
        }
        liangpinjishu = liangpinjishuItem->getValueAsInt();
        auto baojingchixushijianItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$baojingchixushijian$"));
        if (!baojingchixushijianItem) {
            throw std::runtime_error("$variable$baojingchixushijian is not found");
        }
        baojingchixushijian = baojingchixushijianItem->getValueAsDouble();
        auto wenchongzuidahuiduchazhiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$wenchongzuidahuiduchazhi$"));
        if (!wenchongzuidahuiduchazhiItem) {
            throw std::runtime_error("$variable$wenchongzuidahuiduchazhi is not found");
        }
        wenchongzuidahuiduchazhi = wenchongzuidahuiduchazhiItem->getValueAsDouble();
        auto wenchongzuixiaohuiduchazhiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$wenchongzuixiaohuiduchazhi$"));
        if (!wenchongzuixiaohuiduchazhiItem) {
            throw std::runtime_error("$variable$wenchongzuixiaohuiduchazhi is not found");
        }
        wenchongzuixiaohuiduchazhi = wenchongzuixiaohuiduchazhiItem->getValueAsDouble();
        auto wenchongzuidamianjiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$wenchongzuidamianji$"));
        if (!wenchongzuidamianjiItem) {
            throw std::runtime_error("$variable$wenchongzuidamianji is not found");
        }
        wenchongzuidamianji = wenchongzuidamianjiItem->getValueAsDouble();
        auto wenchongzuixiaomianjiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$wenchongzuixiaomianji$"));
        if (!wenchongzuixiaomianjiItem) {
            throw std::runtime_error("$variable$wenchongzuixiaomianji is not found");
        }
        wenchongzuixiaomianji = wenchongzuixiaomianjiItem->getValueAsDouble();
        auto maofazuidahuiduchazhiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$maofazuidahuiduchazhi$"));
        if (!maofazuidahuiduchazhiItem) {
            throw std::runtime_error("$variable$maofazuidahuiduchazhi is not found");
        }
        maofazuidahuiduchazhi = maofazuidahuiduchazhiItem->getValueAsDouble();
        auto maofazuixiaohuiduchazhiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$maofazuixiaohuiduchazhi$"));
        if (!maofazuixiaohuiduchazhiItem) {
            throw std::runtime_error("$variable$maofazuixiaohuiduchazhi is not found");
        }
        maofazuixiaohuiduchazhi = maofazuixiaohuiduchazhiItem->getValueAsDouble();
        auto maofazuidamianjiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$maofazuidamianji$"));
        if (!maofazuidamianjiItem) {
            throw std::runtime_error("$variable$maofazuidamianji is not found");
        }
        maofazuidamianji = maofazuidamianjiItem->getValueAsDouble();
        auto maofazuixiaomianjiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$maofazuixiaomianji$"));
        if (!maofazuixiaomianjiItem) {
            throw std::runtime_error("$variable$maofazuixiaomianji is not found");
        }
        maofazuixiaomianji = maofazuixiaomianjiItem->getValueAsDouble();
    }

    inline SetConfig::SetConfig(const SetConfig& obj)
    {
        tifeichixushijian = obj.tifeichixushijian;
        tifeijuli = obj.tifeijuli;
        shangxiasuojin = obj.shangxiasuojin;
        zuoXianWei = obj.zuoXianWei;
        youXianWei = obj.youXianWei;
        zengyi = obj.zengyi;
        houFenPin = obj.houFenPin;
        chengFaQi = obj.chengFaQi;
        xiangSuDangLiang = obj.xiangSuDangLiang;
        yundongkongzhiqichonglian = obj.yundongkongzhiqichonglian;
        debugMode = obj.debugMode;
        jiaodaitiewanIN = obj.jiaodaitiewanIN;
        tifeixinhaoOUT = obj.tifeixinhaoOUT;
        baojingOUT = obj.baojingOUT;
        lvdengOUT = obj.lvdengOUT;
        hongdengOUT = obj.hongdengOUT;
        xiangjichufachangdu = obj.xiangjichufachangdu;
        meizhuanmaichongshu = obj.meizhuanmaichongshu;
        shedingzhouchang = obj.shedingzhouchang;
        tifeijishu = obj.tifeijishu;
        liangpinjishu = obj.liangpinjishu;
        baojingchixushijian = obj.baojingchixushijian;
        wenchongzuidahuiduchazhi = obj.wenchongzuidahuiduchazhi;
        wenchongzuixiaohuiduchazhi = obj.wenchongzuixiaohuiduchazhi;
        wenchongzuidamianji = obj.wenchongzuidamianji;
        wenchongzuixiaomianji = obj.wenchongzuixiaomianji;
        maofazuidahuiduchazhi = obj.maofazuidahuiduchazhi;
        maofazuixiaohuiduchazhi = obj.maofazuixiaohuiduchazhi;
        maofazuidamianji = obj.maofazuidamianji;
        maofazuixiaomianji = obj.maofazuixiaomianji;
    }

    inline SetConfig& SetConfig::operator=(const SetConfig& obj)
    {
        if (this != &obj) {
            tifeichixushijian = obj.tifeichixushijian;
            tifeijuli = obj.tifeijuli;
            shangxiasuojin = obj.shangxiasuojin;
            zuoXianWei = obj.zuoXianWei;
            youXianWei = obj.youXianWei;
            zengyi = obj.zengyi;
            houFenPin = obj.houFenPin;
            chengFaQi = obj.chengFaQi;
            xiangSuDangLiang = obj.xiangSuDangLiang;
            yundongkongzhiqichonglian = obj.yundongkongzhiqichonglian;
            debugMode = obj.debugMode;
            jiaodaitiewanIN = obj.jiaodaitiewanIN;
            tifeixinhaoOUT = obj.tifeixinhaoOUT;
            baojingOUT = obj.baojingOUT;
            lvdengOUT = obj.lvdengOUT;
            hongdengOUT = obj.hongdengOUT;
            xiangjichufachangdu = obj.xiangjichufachangdu;
            meizhuanmaichongshu = obj.meizhuanmaichongshu;
            shedingzhouchang = obj.shedingzhouchang;
            tifeijishu = obj.tifeijishu;
            liangpinjishu = obj.liangpinjishu;
            baojingchixushijian = obj.baojingchixushijian;
            wenchongzuidahuiduchazhi = obj.wenchongzuidahuiduchazhi;
            wenchongzuixiaohuiduchazhi = obj.wenchongzuixiaohuiduchazhi;
            wenchongzuidamianji = obj.wenchongzuidamianji;
            wenchongzuixiaomianji = obj.wenchongzuixiaomianji;
            maofazuidahuiduchazhi = obj.maofazuidahuiduchazhi;
            maofazuixiaohuiduchazhi = obj.maofazuixiaohuiduchazhi;
            maofazuidamianji = obj.maofazuidamianji;
            maofazuixiaomianji = obj.maofazuixiaomianji;
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
        auto shangxiasuojinItem = std::make_shared<rw::oso::ObjectStoreItem>();
        shangxiasuojinItem->setName("$variable$shangxiasuojin$");
        shangxiasuojinItem->setValueFromDouble(shangxiasuojin);
        assembly.addItem(shangxiasuojinItem);
        auto zuoXianWeiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zuoXianWeiItem->setName("$variable$zuoXianWei$");
        zuoXianWeiItem->setValueFromDouble(zuoXianWei);
        assembly.addItem(zuoXianWeiItem);
        auto youXianWeiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        youXianWeiItem->setName("$variable$youXianWei$");
        youXianWeiItem->setValueFromDouble(youXianWei);
        assembly.addItem(youXianWeiItem);
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
        auto jiaodaitiewanINItem = std::make_shared<rw::oso::ObjectStoreItem>();
        jiaodaitiewanINItem->setName("$variable$jiaodaitiewanIN$");
        jiaodaitiewanINItem->setValueFromInt(jiaodaitiewanIN);
        assembly.addItem(jiaodaitiewanINItem);
        auto tifeixinhaoOUTItem = std::make_shared<rw::oso::ObjectStoreItem>();
        tifeixinhaoOUTItem->setName("$variable$tifeixinhaoOUT$");
        tifeixinhaoOUTItem->setValueFromInt(tifeixinhaoOUT);
        assembly.addItem(tifeixinhaoOUTItem);
        auto baojingOUTItem = std::make_shared<rw::oso::ObjectStoreItem>();
        baojingOUTItem->setName("$variable$baojingOUT$");
        baojingOUTItem->setValueFromInt(baojingOUT);
        assembly.addItem(baojingOUTItem);
        auto lvdengOUTItem = std::make_shared<rw::oso::ObjectStoreItem>();
        lvdengOUTItem->setName("$variable$lvdengOUT$");
        lvdengOUTItem->setValueFromInt(lvdengOUT);
        assembly.addItem(lvdengOUTItem);
        auto hongdengOUTItem = std::make_shared<rw::oso::ObjectStoreItem>();
        hongdengOUTItem->setName("$variable$hongdengOUT$");
        hongdengOUTItem->setValueFromInt(hongdengOUT);
        assembly.addItem(hongdengOUTItem);
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
        auto tifeijishuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        tifeijishuItem->setName("$variable$tifeijishu$");
        tifeijishuItem->setValueFromInt(tifeijishu);
        assembly.addItem(tifeijishuItem);
        auto liangpinjishuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        liangpinjishuItem->setName("$variable$liangpinjishu$");
        liangpinjishuItem->setValueFromInt(liangpinjishu);
        assembly.addItem(liangpinjishuItem);
        auto baojingchixushijianItem = std::make_shared<rw::oso::ObjectStoreItem>();
        baojingchixushijianItem->setName("$variable$baojingchixushijian$");
        baojingchixushijianItem->setValueFromDouble(baojingchixushijian);
        assembly.addItem(baojingchixushijianItem);
        auto wenchongzuidahuiduchazhiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        wenchongzuidahuiduchazhiItem->setName("$variable$wenchongzuidahuiduchazhi$");
        wenchongzuidahuiduchazhiItem->setValueFromDouble(wenchongzuidahuiduchazhi);
        assembly.addItem(wenchongzuidahuiduchazhiItem);
        auto wenchongzuixiaohuiduchazhiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        wenchongzuixiaohuiduchazhiItem->setName("$variable$wenchongzuixiaohuiduchazhi$");
        wenchongzuixiaohuiduchazhiItem->setValueFromDouble(wenchongzuixiaohuiduchazhi);
        assembly.addItem(wenchongzuixiaohuiduchazhiItem);
        auto wenchongzuidamianjiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        wenchongzuidamianjiItem->setName("$variable$wenchongzuidamianji$");
        wenchongzuidamianjiItem->setValueFromDouble(wenchongzuidamianji);
        assembly.addItem(wenchongzuidamianjiItem);
        auto wenchongzuixiaomianjiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        wenchongzuixiaomianjiItem->setName("$variable$wenchongzuixiaomianji$");
        wenchongzuixiaomianjiItem->setValueFromDouble(wenchongzuixiaomianji);
        assembly.addItem(wenchongzuixiaomianjiItem);
        auto maofazuidahuiduchazhiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        maofazuidahuiduchazhiItem->setName("$variable$maofazuidahuiduchazhi$");
        maofazuidahuiduchazhiItem->setValueFromDouble(maofazuidahuiduchazhi);
        assembly.addItem(maofazuidahuiduchazhiItem);
        auto maofazuixiaohuiduchazhiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        maofazuixiaohuiduchazhiItem->setName("$variable$maofazuixiaohuiduchazhi$");
        maofazuixiaohuiduchazhiItem->setValueFromDouble(maofazuixiaohuiduchazhi);
        assembly.addItem(maofazuixiaohuiduchazhiItem);
        auto maofazuidamianjiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        maofazuidamianjiItem->setName("$variable$maofazuidamianji$");
        maofazuidamianjiItem->setValueFromDouble(maofazuidamianji);
        assembly.addItem(maofazuidamianjiItem);
        auto maofazuixiaomianjiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        maofazuixiaomianjiItem->setName("$variable$maofazuixiaomianji$");
        maofazuixiaomianjiItem->setValueFromDouble(maofazuixiaomianji);
        assembly.addItem(maofazuixiaomianjiItem);
        return assembly;
    }

    inline bool SetConfig::operator==(const SetConfig& obj) const
    {
        return tifeichixushijian == obj.tifeichixushijian && tifeijuli == obj.tifeijuli && shangxiasuojin == obj.shangxiasuojin && zuoXianWei == obj.zuoXianWei && youXianWei == obj.youXianWei && zengyi == obj.zengyi && houFenPin == obj.houFenPin && chengFaQi == obj.chengFaQi && xiangSuDangLiang == obj.xiangSuDangLiang && yundongkongzhiqichonglian == obj.yundongkongzhiqichonglian && debugMode == obj.debugMode && jiaodaitiewanIN == obj.jiaodaitiewanIN && tifeixinhaoOUT == obj.tifeixinhaoOUT && baojingOUT == obj.baojingOUT && lvdengOUT == obj.lvdengOUT && hongdengOUT == obj.hongdengOUT && xiangjichufachangdu == obj.xiangjichufachangdu && meizhuanmaichongshu == obj.meizhuanmaichongshu && shedingzhouchang == obj.shedingzhouchang && tifeijishu == obj.tifeijishu && liangpinjishu == obj.liangpinjishu && baojingchixushijian == obj.baojingchixushijian && wenchongzuidahuiduchazhi == obj.wenchongzuidahuiduchazhi && wenchongzuixiaohuiduchazhi == obj.wenchongzuixiaohuiduchazhi && wenchongzuidamianji == obj.wenchongzuidamianji && wenchongzuixiaomianji == obj.wenchongzuixiaomianji && maofazuidahuiduchazhi == obj.maofazuidahuiduchazhi && maofazuixiaohuiduchazhi == obj.maofazuixiaohuiduchazhi && maofazuidamianji == obj.maofazuidamianji && maofazuixiaomianji == obj.maofazuixiaomianji;
    }

    inline bool SetConfig::operator!=(const SetConfig& obj) const
    {
        return !(*this == obj);
    }

}

