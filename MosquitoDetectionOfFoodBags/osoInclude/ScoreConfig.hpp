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
        bool test{ false };
    };

    inline ScoreConfig::ScoreConfig(const rw::oso::ObjectStoreAssembly& assembly)
    {
        auto isAccountAssembly = assembly.getName();
        if (isAccountAssembly != "$class$ScoreConfig$")
        {
            throw std::runtime_error("Assembly is not $class$ScoreConfig$");
        }
        auto testItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$test$"));
        if (!testItem) {
            throw std::runtime_error("$variable$test is not found");
        }
        test = testItem->getValueAsBool();
    }

    inline ScoreConfig::ScoreConfig(const ScoreConfig& obj)
    {
        test = obj.test;
    }

    inline ScoreConfig& ScoreConfig::operator=(const ScoreConfig& obj)
    {
        if (this != &obj) {
            test = obj.test;
        }
        return *this;
    }

    inline ScoreConfig::operator rw::oso::ObjectStoreAssembly() const
    {
        rw::oso::ObjectStoreAssembly assembly;
        assembly.setName("$class$ScoreConfig$");
        auto testItem = std::make_shared<rw::oso::ObjectStoreItem>();
        testItem->setName("$variable$test$");
        testItem->setValueFromBool(test);
        assembly.addItem(testItem);
        return assembly;
    }

    inline bool ScoreConfig::operator==(const ScoreConfig& obj) const
    {
        return test == obj.test;
    }

    inline bool ScoreConfig::operator!=(const ScoreConfig& obj) const
    {
        return !(*this == obj);
    }

}

