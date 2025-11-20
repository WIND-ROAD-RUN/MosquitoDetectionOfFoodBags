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
        int defectIgnoreX{ 0 };
        int defectIgnoreY{ 0 };
        double shangXianWei1{ 0 };
        double xiaXianWei1{ 0 };
        double zuoXianWei1{ 0 };
        double youXianWei1{ 0 };
        double baoguang1{ 0 };
        double zengyi1{ 0 };
        double xiangSuDangLiang1{ 0 };
        bool takeWork1Pictures{ false };
        bool takeWork2Pictures{ false };
        bool saveNGImg{ false };
        bool saveMaskImg{ false };
        bool saveOKImg{ false };
        bool qiyongerxiangji{ false };
        bool qiyongyundongkongzhiqi{ false };
        bool yundongkongzhiqichonglian{ false };
        bool debugMode{ false };
        int tifeixinhaoOUT{ 0 };
        int xiangjichufa1OUT{ 0 };
        int xiangjichufa2OUT{ 0 };
        double xiangjichufachangdu{ 0 };
        double meizhuanmaichongshu{ 0 };
        double shedingzhouchang{ 0 };
        bool isZangWu{ false };
        bool isXiaoZangWu{ false };
        bool isJieTou{ false };
        bool isBody{ false };
        bool isHuaPo{ false };
        bool isWeiZhiQueXian{ false };
        bool isTichuDongzuo{ true };
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
        auto defectIgnoreXItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$defectIgnoreX$"));
        if (!defectIgnoreXItem) {
            throw std::runtime_error("$variable$defectIgnoreX is not found");
        }
        defectIgnoreX = defectIgnoreXItem->getValueAsInt();
        auto defectIgnoreYItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$defectIgnoreY$"));
        if (!defectIgnoreYItem) {
            throw std::runtime_error("$variable$defectIgnoreY is not found");
        }
        defectIgnoreY = defectIgnoreYItem->getValueAsInt();
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
        auto takeWork1PicturesItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$takeWork1Pictures$"));
        if (!takeWork1PicturesItem) {
            throw std::runtime_error("$variable$takeWork1Pictures is not found");
        }
        takeWork1Pictures = takeWork1PicturesItem->getValueAsBool();
        auto takeWork2PicturesItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$takeWork2Pictures$"));
        if (!takeWork2PicturesItem) {
            throw std::runtime_error("$variable$takeWork2Pictures is not found");
        }
        takeWork2Pictures = takeWork2PicturesItem->getValueAsBool();
        auto saveNGImgItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$saveNGImg$"));
        if (!saveNGImgItem) {
            throw std::runtime_error("$variable$saveNGImg is not found");
        }
        saveNGImg = saveNGImgItem->getValueAsBool();
        auto saveMaskImgItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$saveMaskImg$"));
        if (!saveMaskImgItem) {
            throw std::runtime_error("$variable$saveMaskImg is not found");
        }
        saveMaskImg = saveMaskImgItem->getValueAsBool();
        auto saveOKImgItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$saveOKImg$"));
        if (!saveOKImgItem) {
            throw std::runtime_error("$variable$saveOKImg is not found");
        }
        saveOKImg = saveOKImgItem->getValueAsBool();
        auto qiyongerxiangjiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$qiyongerxiangji$"));
        if (!qiyongerxiangjiItem) {
            throw std::runtime_error("$variable$qiyongerxiangji is not found");
        }
        qiyongerxiangji = qiyongerxiangjiItem->getValueAsBool();
        auto qiyongyundongkongzhiqiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$qiyongyundongkongzhiqi$"));
        if (!qiyongyundongkongzhiqiItem) {
            throw std::runtime_error("$variable$qiyongyundongkongzhiqi is not found");
        }
        qiyongyundongkongzhiqi = qiyongyundongkongzhiqiItem->getValueAsBool();
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
        auto isZangWuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isZangWu$"));
        if (!isZangWuItem) {
            throw std::runtime_error("$variable$isZangWu is not found");
        }
        isZangWu = isZangWuItem->getValueAsBool();
        auto isXiaoZangWuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isXiaoZangWu$"));
        if (!isXiaoZangWuItem) {
            throw std::runtime_error("$variable$isXiaoZangWu is not found");
        }
        isXiaoZangWu = isXiaoZangWuItem->getValueAsBool();
        auto isJieTouItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isJieTou$"));
        if (!isJieTouItem) {
            throw std::runtime_error("$variable$isJieTou is not found");
        }
        isJieTou = isJieTouItem->getValueAsBool();
        auto isBodyItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isBody$"));
        if (!isBodyItem) {
            throw std::runtime_error("$variable$isBody is not found");
        }
        isBody = isBodyItem->getValueAsBool();
        auto isHuaPoItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isHuaPo$"));
        if (!isHuaPoItem) {
            throw std::runtime_error("$variable$isHuaPo is not found");
        }
        isHuaPo = isHuaPoItem->getValueAsBool();
        auto isWeiZhiQueXianItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isWeiZhiQueXian$"));
        if (!isWeiZhiQueXianItem) {
            throw std::runtime_error("$variable$isWeiZhiQueXian is not found");
        }
        isWeiZhiQueXian = isWeiZhiQueXianItem->getValueAsBool();
        auto isTichuDongzuoItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isTichuDongzuo$"));
        if (!isTichuDongzuoItem) {
            throw std::runtime_error("$variable$isTichuDongzuo is not found");
        }
        isTichuDongzuo = isTichuDongzuoItem->getValueAsBool();
    }

    inline SetConfig::SetConfig(const SetConfig& obj)
    {
        tifeichixushijian1 = obj.tifeichixushijian1;
        tifeijuli1 = obj.tifeijuli1;
        defectIgnoreX = obj.defectIgnoreX;
        defectIgnoreY = obj.defectIgnoreY;
        shangXianWei1 = obj.shangXianWei1;
        xiaXianWei1 = obj.xiaXianWei1;
        zuoXianWei1 = obj.zuoXianWei1;
        youXianWei1 = obj.youXianWei1;
        baoguang1 = obj.baoguang1;
        zengyi1 = obj.zengyi1;
        xiangSuDangLiang1 = obj.xiangSuDangLiang1;
        takeWork1Pictures = obj.takeWork1Pictures;
        takeWork2Pictures = obj.takeWork2Pictures;
        saveNGImg = obj.saveNGImg;
        saveMaskImg = obj.saveMaskImg;
        saveOKImg = obj.saveOKImg;
        qiyongerxiangji = obj.qiyongerxiangji;
        qiyongyundongkongzhiqi = obj.qiyongyundongkongzhiqi;
        yundongkongzhiqichonglian = obj.yundongkongzhiqichonglian;
        debugMode = obj.debugMode;
        tifeixinhaoOUT = obj.tifeixinhaoOUT;
        xiangjichufa1OUT = obj.xiangjichufa1OUT;
        xiangjichufa2OUT = obj.xiangjichufa2OUT;
        xiangjichufachangdu = obj.xiangjichufachangdu;
        meizhuanmaichongshu = obj.meizhuanmaichongshu;
        shedingzhouchang = obj.shedingzhouchang;
        isZangWu = obj.isZangWu;
        isXiaoZangWu = obj.isXiaoZangWu;
        isJieTou = obj.isJieTou;
        isBody = obj.isBody;
        isHuaPo = obj.isHuaPo;
        isWeiZhiQueXian = obj.isWeiZhiQueXian;
        isTichuDongzuo = obj.isTichuDongzuo;
    }

    inline SetConfig& SetConfig::operator=(const SetConfig& obj)
    {
        if (this != &obj) {
            tifeichixushijian1 = obj.tifeichixushijian1;
            tifeijuli1 = obj.tifeijuli1;
            defectIgnoreX = obj.defectIgnoreX;
            defectIgnoreY = obj.defectIgnoreY;
            shangXianWei1 = obj.shangXianWei1;
            xiaXianWei1 = obj.xiaXianWei1;
            zuoXianWei1 = obj.zuoXianWei1;
            youXianWei1 = obj.youXianWei1;
            baoguang1 = obj.baoguang1;
            zengyi1 = obj.zengyi1;
            xiangSuDangLiang1 = obj.xiangSuDangLiang1;
            takeWork1Pictures = obj.takeWork1Pictures;
            takeWork2Pictures = obj.takeWork2Pictures;
            saveNGImg = obj.saveNGImg;
            saveMaskImg = obj.saveMaskImg;
            saveOKImg = obj.saveOKImg;
            qiyongerxiangji = obj.qiyongerxiangji;
            qiyongyundongkongzhiqi = obj.qiyongyundongkongzhiqi;
            yundongkongzhiqichonglian = obj.yundongkongzhiqichonglian;
            debugMode = obj.debugMode;
            tifeixinhaoOUT = obj.tifeixinhaoOUT;
            xiangjichufa1OUT = obj.xiangjichufa1OUT;
            xiangjichufa2OUT = obj.xiangjichufa2OUT;
            xiangjichufachangdu = obj.xiangjichufachangdu;
            meizhuanmaichongshu = obj.meizhuanmaichongshu;
            shedingzhouchang = obj.shedingzhouchang;
            isZangWu = obj.isZangWu;
            isXiaoZangWu = obj.isXiaoZangWu;
            isJieTou = obj.isJieTou;
            isBody = obj.isBody;
            isHuaPo = obj.isHuaPo;
            isWeiZhiQueXian = obj.isWeiZhiQueXian;
            isTichuDongzuo = obj.isTichuDongzuo;
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
        auto defectIgnoreXItem = std::make_shared<rw::oso::ObjectStoreItem>();
        defectIgnoreXItem->setName("$variable$defectIgnoreX$");
        defectIgnoreXItem->setValueFromInt(defectIgnoreX);
        assembly.addItem(defectIgnoreXItem);
        auto defectIgnoreYItem = std::make_shared<rw::oso::ObjectStoreItem>();
        defectIgnoreYItem->setName("$variable$defectIgnoreY$");
        defectIgnoreYItem->setValueFromInt(defectIgnoreY);
        assembly.addItem(defectIgnoreYItem);
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
        auto takeWork1PicturesItem = std::make_shared<rw::oso::ObjectStoreItem>();
        takeWork1PicturesItem->setName("$variable$takeWork1Pictures$");
        takeWork1PicturesItem->setValueFromBool(takeWork1Pictures);
        assembly.addItem(takeWork1PicturesItem);
        auto takeWork2PicturesItem = std::make_shared<rw::oso::ObjectStoreItem>();
        takeWork2PicturesItem->setName("$variable$takeWork2Pictures$");
        takeWork2PicturesItem->setValueFromBool(takeWork2Pictures);
        assembly.addItem(takeWork2PicturesItem);
        auto saveNGImgItem = std::make_shared<rw::oso::ObjectStoreItem>();
        saveNGImgItem->setName("$variable$saveNGImg$");
        saveNGImgItem->setValueFromBool(saveNGImg);
        assembly.addItem(saveNGImgItem);
        auto saveMaskImgItem = std::make_shared<rw::oso::ObjectStoreItem>();
        saveMaskImgItem->setName("$variable$saveMaskImg$");
        saveMaskImgItem->setValueFromBool(saveMaskImg);
        assembly.addItem(saveMaskImgItem);
        auto saveOKImgItem = std::make_shared<rw::oso::ObjectStoreItem>();
        saveOKImgItem->setName("$variable$saveOKImg$");
        saveOKImgItem->setValueFromBool(saveOKImg);
        assembly.addItem(saveOKImgItem);
        auto qiyongerxiangjiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        qiyongerxiangjiItem->setName("$variable$qiyongerxiangji$");
        qiyongerxiangjiItem->setValueFromBool(qiyongerxiangji);
        assembly.addItem(qiyongerxiangjiItem);
        auto qiyongyundongkongzhiqiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        qiyongyundongkongzhiqiItem->setName("$variable$qiyongyundongkongzhiqi$");
        qiyongyundongkongzhiqiItem->setValueFromBool(qiyongyundongkongzhiqi);
        assembly.addItem(qiyongyundongkongzhiqiItem);
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
        auto isZangWuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isZangWuItem->setName("$variable$isZangWu$");
        isZangWuItem->setValueFromBool(isZangWu);
        assembly.addItem(isZangWuItem);
        auto isXiaoZangWuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isXiaoZangWuItem->setName("$variable$isXiaoZangWu$");
        isXiaoZangWuItem->setValueFromBool(isXiaoZangWu);
        assembly.addItem(isXiaoZangWuItem);
        auto isJieTouItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isJieTouItem->setName("$variable$isJieTou$");
        isJieTouItem->setValueFromBool(isJieTou);
        assembly.addItem(isJieTouItem);
        auto isBodyItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isBodyItem->setName("$variable$isBody$");
        isBodyItem->setValueFromBool(isBody);
        assembly.addItem(isBodyItem);
        auto isHuaPoItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isHuaPoItem->setName("$variable$isHuaPo$");
        isHuaPoItem->setValueFromBool(isHuaPo);
        assembly.addItem(isHuaPoItem);
        auto isWeiZhiQueXianItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isWeiZhiQueXianItem->setName("$variable$isWeiZhiQueXian$");
        isWeiZhiQueXianItem->setValueFromBool(isWeiZhiQueXian);
        assembly.addItem(isWeiZhiQueXianItem);
        auto isTichuDongzuoItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isTichuDongzuoItem->setName("$variable$isTichuDongzuo$");
        isTichuDongzuoItem->setValueFromBool(isTichuDongzuo);
        assembly.addItem(isTichuDongzuoItem);
        return assembly;
    }

    inline bool SetConfig::operator==(const SetConfig& obj) const
    {
        return tifeichixushijian1 == obj.tifeichixushijian1 && tifeijuli1 == obj.tifeijuli1 && defectIgnoreX == obj.defectIgnoreX && defectIgnoreY == obj.defectIgnoreY && shangXianWei1 == obj.shangXianWei1 && xiaXianWei1 == obj.xiaXianWei1 && zuoXianWei1 == obj.zuoXianWei1 && youXianWei1 == obj.youXianWei1 && baoguang1 == obj.baoguang1 && zengyi1 == obj.zengyi1 && xiangSuDangLiang1 == obj.xiangSuDangLiang1 && takeWork1Pictures == obj.takeWork1Pictures && takeWork2Pictures == obj.takeWork2Pictures && saveNGImg == obj.saveNGImg && saveMaskImg == obj.saveMaskImg && saveOKImg == obj.saveOKImg && qiyongerxiangji == obj.qiyongerxiangji && qiyongyundongkongzhiqi == obj.qiyongyundongkongzhiqi && yundongkongzhiqichonglian == obj.yundongkongzhiqichonglian && debugMode == obj.debugMode && tifeixinhaoOUT == obj.tifeixinhaoOUT && xiangjichufa1OUT == obj.xiangjichufa1OUT && xiangjichufa2OUT == obj.xiangjichufa2OUT && xiangjichufachangdu == obj.xiangjichufachangdu && meizhuanmaichongshu == obj.meizhuanmaichongshu && shedingzhouchang == obj.shedingzhouchang && isZangWu == obj.isZangWu && isXiaoZangWu == obj.isXiaoZangWu && isJieTou == obj.isJieTou && isBody == obj.isBody && isHuaPo == obj.isHuaPo && isWeiZhiQueXian == obj.isWeiZhiQueXian && isTichuDongzuo == obj.isTichuDongzuo;
    }

    inline bool SetConfig::operator!=(const SetConfig& obj) const
    {
        return !(*this == obj);
    }

}

