#pragma once

#include"oso_core.h"
#include <string>

namespace cdm {
    class ScoreConfig
    {
    public:
        ScoreConfig() = default;
        ~ScoreConfig() = default;

        ScoreConfig(const rw::oso::ObjectStoreAssembly& assembly);
        ScoreConfig(const ScoreConfig& obj);

        ScoreConfig& operator=(const ScoreConfig& obj);
        operator rw::oso::ObjectStoreAssembly() const;
        bool operator==(const ScoreConfig& obj) const;
        bool operator!=(const ScoreConfig& obj) const;

    public:
        bool zangWu{ false };
        double zangWuScore{ 0 };
        double zangWuArea{ 0 };
        bool jieTou{ false };
        double jieTouScore{ 0 };
        double jieTouArea{ 0 };
        bool xiaoZangWu{ false };
        double xiaoZangWuScore{ 0 };
        double xiaoZangWuArea{ 0 };
        bool body{ false };
        double bodyScore{ 0 };
        double bodyArea{ 0 };
        bool huaPo{ false };
        double huaPoScore{ 0 };
        double huaPoArea{ 0 };
        bool weiZhiQueXianSingle{ false };
        double weiZhiQueXianSingleArea{ 0 };
        bool weiZhiQueXianAll{ false };
        double weiZhiQueXianAllArea{ 0 };
    };

    inline ScoreConfig::ScoreConfig(const rw::oso::ObjectStoreAssembly& assembly)
    {
        auto isAccountAssembly = assembly.getName();
        if (isAccountAssembly != "$class$ScoreConfig$")
        {
            throw std::runtime_error("Assembly is not $class$ScoreConfig$");
        }
        auto zangWuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zangWu$"));
        if (!zangWuItem) {
            throw std::runtime_error("$variable$zangWu is not found");
        }
        zangWu = zangWuItem->getValueAsBool();
        auto zangWuScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zangWuScore$"));
        if (!zangWuScoreItem) {
            throw std::runtime_error("$variable$zangWuScore is not found");
        }
        zangWuScore = zangWuScoreItem->getValueAsDouble();
        auto zangWuAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zangWuArea$"));
        if (!zangWuAreaItem) {
            throw std::runtime_error("$variable$zangWuArea is not found");
        }
        zangWuArea = zangWuAreaItem->getValueAsDouble();
        auto jieTouItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$jieTou$"));
        if (!jieTouItem) {
            throw std::runtime_error("$variable$jieTou is not found");
        }
        jieTou = jieTouItem->getValueAsBool();
        auto jieTouScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$jieTouScore$"));
        if (!jieTouScoreItem) {
            throw std::runtime_error("$variable$jieTouScore is not found");
        }
        jieTouScore = jieTouScoreItem->getValueAsDouble();
        auto jieTouAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$jieTouArea$"));
        if (!jieTouAreaItem) {
            throw std::runtime_error("$variable$jieTouArea is not found");
        }
        jieTouArea = jieTouAreaItem->getValueAsDouble();
        auto xiaoZangWuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiaoZangWu$"));
        if (!xiaoZangWuItem) {
            throw std::runtime_error("$variable$xiaoZangWu is not found");
        }
        xiaoZangWu = xiaoZangWuItem->getValueAsBool();
        auto xiaoZangWuScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiaoZangWuScore$"));
        if (!xiaoZangWuScoreItem) {
            throw std::runtime_error("$variable$xiaoZangWuScore is not found");
        }
        xiaoZangWuScore = xiaoZangWuScoreItem->getValueAsDouble();
        auto xiaoZangWuAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiaoZangWuArea$"));
        if (!xiaoZangWuAreaItem) {
            throw std::runtime_error("$variable$xiaoZangWuArea is not found");
        }
        xiaoZangWuArea = xiaoZangWuAreaItem->getValueAsDouble();
        auto bodyItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$body$"));
        if (!bodyItem) {
            throw std::runtime_error("$variable$body is not found");
        }
        body = bodyItem->getValueAsBool();
        auto bodyScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$bodyScore$"));
        if (!bodyScoreItem) {
            throw std::runtime_error("$variable$bodyScore is not found");
        }
        bodyScore = bodyScoreItem->getValueAsDouble();
        auto bodyAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$bodyArea$"));
        if (!bodyAreaItem) {
            throw std::runtime_error("$variable$bodyArea is not found");
        }
        bodyArea = bodyAreaItem->getValueAsDouble();
        auto huaPoItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$huaPo$"));
        if (!huaPoItem) {
            throw std::runtime_error("$variable$huaPo is not found");
        }
        huaPo = huaPoItem->getValueAsBool();
        auto huaPoScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$huaPoScore$"));
        if (!huaPoScoreItem) {
            throw std::runtime_error("$variable$huaPoScore is not found");
        }
        huaPoScore = huaPoScoreItem->getValueAsDouble();
        auto huaPoAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$huaPoArea$"));
        if (!huaPoAreaItem) {
            throw std::runtime_error("$variable$huaPoArea is not found");
        }
        huaPoArea = huaPoAreaItem->getValueAsDouble();
        auto weiZhiQueXianSingleItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$weiZhiQueXianSingle$"));
        if (!weiZhiQueXianSingleItem) {
            throw std::runtime_error("$variable$weiZhiQueXianSingle is not found");
        }
        weiZhiQueXianSingle = weiZhiQueXianSingleItem->getValueAsBool();
        auto weiZhiQueXianSingleAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$weiZhiQueXianSingleArea$"));
        if (!weiZhiQueXianSingleAreaItem) {
            throw std::runtime_error("$variable$weiZhiQueXianSingleArea is not found");
        }
        weiZhiQueXianSingleArea = weiZhiQueXianSingleAreaItem->getValueAsDouble();
        auto weiZhiQueXianAllItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$weiZhiQueXianAll$"));
        if (!weiZhiQueXianAllItem) {
            throw std::runtime_error("$variable$weiZhiQueXianAll is not found");
        }
        weiZhiQueXianAll = weiZhiQueXianAllItem->getValueAsBool();
        auto weiZhiQueXianAllAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$weiZhiQueXianAllArea$"));
        if (!weiZhiQueXianAllAreaItem) {
            throw std::runtime_error("$variable$weiZhiQueXianAllArea is not found");
        }
        weiZhiQueXianAllArea = weiZhiQueXianAllAreaItem->getValueAsDouble();
    }

    inline ScoreConfig::ScoreConfig(const ScoreConfig& obj)
    {
        zangWu = obj.zangWu;
        zangWuScore = obj.zangWuScore;
        zangWuArea = obj.zangWuArea;
        jieTou = obj.jieTou;
        jieTouScore = obj.jieTouScore;
        jieTouArea = obj.jieTouArea;
        xiaoZangWu = obj.xiaoZangWu;
        xiaoZangWuScore = obj.xiaoZangWuScore;
        xiaoZangWuArea = obj.xiaoZangWuArea;
        body = obj.body;
        bodyScore = obj.bodyScore;
        bodyArea = obj.bodyArea;
        huaPo = obj.huaPo;
        huaPoScore = obj.huaPoScore;
        huaPoArea = obj.huaPoArea;
        weiZhiQueXianSingle = obj.weiZhiQueXianSingle;
        weiZhiQueXianSingleArea = obj.weiZhiQueXianSingleArea;
        weiZhiQueXianAll = obj.weiZhiQueXianAll;
        weiZhiQueXianAllArea = obj.weiZhiQueXianAllArea;
    }

    inline ScoreConfig& ScoreConfig::operator=(const ScoreConfig& obj)
    {
        if (this != &obj) {
            zangWu = obj.zangWu;
            zangWuScore = obj.zangWuScore;
            zangWuArea = obj.zangWuArea;
            jieTou = obj.jieTou;
            jieTouScore = obj.jieTouScore;
            jieTouArea = obj.jieTouArea;
            xiaoZangWu = obj.xiaoZangWu;
            xiaoZangWuScore = obj.xiaoZangWuScore;
            xiaoZangWuArea = obj.xiaoZangWuArea;
            body = obj.body;
            bodyScore = obj.bodyScore;
            bodyArea = obj.bodyArea;
            huaPo = obj.huaPo;
            huaPoScore = obj.huaPoScore;
            huaPoArea = obj.huaPoArea;
            weiZhiQueXianSingle = obj.weiZhiQueXianSingle;
            weiZhiQueXianSingleArea = obj.weiZhiQueXianSingleArea;
            weiZhiQueXianAll = obj.weiZhiQueXianAll;
            weiZhiQueXianAllArea = obj.weiZhiQueXianAllArea;
        }
        return *this;
    }

    inline ScoreConfig::operator rw::oso::ObjectStoreAssembly() const
    {
        rw::oso::ObjectStoreAssembly assembly;
        assembly.setName("$class$ScoreConfig$");
        auto zangWuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zangWuItem->setName("$variable$zangWu$");
        zangWuItem->setValueFromBool(zangWu);
        assembly.addItem(zangWuItem);
        auto zangWuScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zangWuScoreItem->setName("$variable$zangWuScore$");
        zangWuScoreItem->setValueFromDouble(zangWuScore);
        assembly.addItem(zangWuScoreItem);
        auto zangWuAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zangWuAreaItem->setName("$variable$zangWuArea$");
        zangWuAreaItem->setValueFromDouble(zangWuArea);
        assembly.addItem(zangWuAreaItem);
        auto jieTouItem = std::make_shared<rw::oso::ObjectStoreItem>();
        jieTouItem->setName("$variable$jieTou$");
        jieTouItem->setValueFromBool(jieTou);
        assembly.addItem(jieTouItem);
        auto jieTouScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        jieTouScoreItem->setName("$variable$jieTouScore$");
        jieTouScoreItem->setValueFromDouble(jieTouScore);
        assembly.addItem(jieTouScoreItem);
        auto jieTouAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        jieTouAreaItem->setName("$variable$jieTouArea$");
        jieTouAreaItem->setValueFromDouble(jieTouArea);
        assembly.addItem(jieTouAreaItem);
        auto xiaoZangWuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiaoZangWuItem->setName("$variable$xiaoZangWu$");
        xiaoZangWuItem->setValueFromBool(xiaoZangWu);
        assembly.addItem(xiaoZangWuItem);
        auto xiaoZangWuScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiaoZangWuScoreItem->setName("$variable$xiaoZangWuScore$");
        xiaoZangWuScoreItem->setValueFromDouble(xiaoZangWuScore);
        assembly.addItem(xiaoZangWuScoreItem);
        auto xiaoZangWuAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiaoZangWuAreaItem->setName("$variable$xiaoZangWuArea$");
        xiaoZangWuAreaItem->setValueFromDouble(xiaoZangWuArea);
        assembly.addItem(xiaoZangWuAreaItem);
        auto bodyItem = std::make_shared<rw::oso::ObjectStoreItem>();
        bodyItem->setName("$variable$body$");
        bodyItem->setValueFromBool(body);
        assembly.addItem(bodyItem);
        auto bodyScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        bodyScoreItem->setName("$variable$bodyScore$");
        bodyScoreItem->setValueFromDouble(bodyScore);
        assembly.addItem(bodyScoreItem);
        auto bodyAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        bodyAreaItem->setName("$variable$bodyArea$");
        bodyAreaItem->setValueFromDouble(bodyArea);
        assembly.addItem(bodyAreaItem);
        auto huaPoItem = std::make_shared<rw::oso::ObjectStoreItem>();
        huaPoItem->setName("$variable$huaPo$");
        huaPoItem->setValueFromBool(huaPo);
        assembly.addItem(huaPoItem);
        auto huaPoScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        huaPoScoreItem->setName("$variable$huaPoScore$");
        huaPoScoreItem->setValueFromDouble(huaPoScore);
        assembly.addItem(huaPoScoreItem);
        auto huaPoAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        huaPoAreaItem->setName("$variable$huaPoArea$");
        huaPoAreaItem->setValueFromDouble(huaPoArea);
        assembly.addItem(huaPoAreaItem);
        auto weiZhiQueXianSingleItem = std::make_shared<rw::oso::ObjectStoreItem>();
        weiZhiQueXianSingleItem->setName("$variable$weiZhiQueXianSingle$");
        weiZhiQueXianSingleItem->setValueFromBool(weiZhiQueXianSingle);
        assembly.addItem(weiZhiQueXianSingleItem);
        auto weiZhiQueXianSingleAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        weiZhiQueXianSingleAreaItem->setName("$variable$weiZhiQueXianSingleArea$");
        weiZhiQueXianSingleAreaItem->setValueFromDouble(weiZhiQueXianSingleArea);
        assembly.addItem(weiZhiQueXianSingleAreaItem);
        auto weiZhiQueXianAllItem = std::make_shared<rw::oso::ObjectStoreItem>();
        weiZhiQueXianAllItem->setName("$variable$weiZhiQueXianAll$");
        weiZhiQueXianAllItem->setValueFromBool(weiZhiQueXianAll);
        assembly.addItem(weiZhiQueXianAllItem);
        auto weiZhiQueXianAllAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        weiZhiQueXianAllAreaItem->setName("$variable$weiZhiQueXianAllArea$");
        weiZhiQueXianAllAreaItem->setValueFromDouble(weiZhiQueXianAllArea);
        assembly.addItem(weiZhiQueXianAllAreaItem);
        return assembly;
    }

    inline bool ScoreConfig::operator==(const ScoreConfig& obj) const
    {
        return zangWu == obj.zangWu && zangWuScore == obj.zangWuScore && zangWuArea == obj.zangWuArea && jieTou == obj.jieTou && jieTouScore == obj.jieTouScore && jieTouArea == obj.jieTouArea && xiaoZangWu == obj.xiaoZangWu && xiaoZangWuScore == obj.xiaoZangWuScore && xiaoZangWuArea == obj.xiaoZangWuArea && body == obj.body && bodyScore == obj.bodyScore && bodyArea == obj.bodyArea && huaPo == obj.huaPo && huaPoScore == obj.huaPoScore && huaPoArea == obj.huaPoArea && weiZhiQueXianSingle == obj.weiZhiQueXianSingle && weiZhiQueXianSingleArea == obj.weiZhiQueXianSingleArea && weiZhiQueXianAll == obj.weiZhiQueXianAll && weiZhiQueXianAllArea == obj.weiZhiQueXianAllArea;
    }

    inline bool ScoreConfig::operator!=(const ScoreConfig& obj) const
    {
        return !(*this == obj);
    }

}

