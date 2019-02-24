#include "spell_checker.h"

#include <fstream>
#include <memory>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <unordered_set>

static constexpr size_t WORD_MAX_SIZE = 45;

class SpellChecker_VectorImpl : public SpellChecker_Impl {
public:
    SpellChecker_VectorImpl(){};
    void load (const std::string& dictionary) override;
    bool check(const std::string& word) const override;
    void add(const std::string& word) override;
    size_t size(void) const override;
private:
    std::vector<std::string> dict;
};

class SpellChecker_SetImpl : public SpellChecker_Impl {
public:
    SpellChecker_SetImpl(){};
    void load (const std::string& dictionary) override;
    void add(const std::string& word) override;
    bool check(const std::string& word) const override;
    size_t size(void) const override;
private:
    std::set<std::string> dict;
};

class SpellChecker_UnSetImpl : public SpellChecker_Impl {
public:
    SpellChecker_UnSetImpl(){};
    void load (const std::string& dictionary) override;
    void add(const std::string& word) override;
    bool check(const std::string& word) const override;
    size_t size(void) const override;
private:
    std::unordered_set<std::string> dict;
};
//================================================================================================
SpellChecker::SpellChecker(const ContainerType type) {
    switch (type) {
        case ContainerType::Vector : impl_ = std::make_unique<SpellChecker_VectorImpl>();break;
        case ContainerType::Set : impl_ = std::make_unique<SpellChecker_SetImpl>();break;
        case ContainerType::Unordered_Set : impl_ = std::make_unique<SpellChecker_UnSetImpl>();break;
    }
}

void SpellChecker::load(const std::string& dictionary) {
    impl_->load(dictionary);
}

void SpellChecker::add(const std::string& word) {
    impl_->add(word);
}

bool SpellChecker::check(const std::string& word) const {
    if (is_valid(word))
        return impl_->check(word);
    return false;
}

size_t SpellChecker::size(void) const {
    return impl_->size();
}

bool SpellChecker::is_valid(const std::string& word) {
    if (word.size() > WORD_MAX_SIZE) {
        return false;
    }
    if (word[0] == '\'') {
        return false;
    }
    for (auto i = word.cbegin(); i != word.cend(); i++) {
        if (!std::isalpha(*i) && *i != '\'') {
            return false;
        }
    }
    return true;
}

//===========================================================

void SpellChecker_VectorImpl::load(const std::string& dictionary) {
    std::ifstream file(dictionary);
    std::string temp;
    if (!file.is_open())
        throw SpellChecker_InvalidDictFile();
    while(file >> temp) {
        dict.insert(dict.end(), temp);
    }
}

void SpellChecker_VectorImpl::add(const std::string& word) {
    if (!check(word)) {
        dict.insert(std::lower_bound(dict.begin(), dict.end(), word), word);
    }
}

bool SpellChecker_VectorImpl::check(const std::string& word) const {
    std::string temp(word);
    std::transform(word.begin(), word.end(), temp.begin(), ::tolower);
    return std::binary_search(dict.begin(), dict.end(), temp);
}

size_t SpellChecker_VectorImpl::size(void) const {
    return dict.size();
}

//==========================================================

void SpellChecker_SetImpl::load(const std::string& dictionary) {
    std::ifstream file(dictionary);
    std::string temp;
    if (!file.is_open())
        throw SpellChecker_InvalidDictFile();
    while(file >> temp) {
        dict.insert(temp);
    }
}

void SpellChecker_SetImpl::add(const std::string& word) {
    dict.insert(word);
}

bool SpellChecker_SetImpl::check(const std::string& word) const {
    std::string temp(word);
    std::transform(word.begin(), word.end(), temp.begin(), ::tolower);
    auto search = dict.find(temp);
    if (search != dict.end())
        return true;
    return false;
}

size_t SpellChecker_SetImpl::size(void) const {
    return dict.size();
}

//==========================================================

void SpellChecker_UnSetImpl::load(const std::string& dictionary) {
    std::ifstream file(dictionary);
    std::string temp;
    if (!file.is_open())
        throw SpellChecker_InvalidDictFile();
    while(file >> temp) {
        dict.insert(temp);
    }
}

void SpellChecker_UnSetImpl::add(const std::string& word) {
    dict.insert(word);
}

bool SpellChecker_UnSetImpl::check(const std::string& word) const {
    std::string temp(word);
    std::transform(word.begin(), word.end(), temp.begin(), ::tolower);
    auto search = dict.find(temp);
    if (search != dict.end())
        return true;
    return false;
}

size_t SpellChecker_UnSetImpl::size(void) const {
    return dict.size();
}
