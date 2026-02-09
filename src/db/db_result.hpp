#ifndef DB_RESULT_H
#define DB_RESULT_H
#pragma once
#include "mysql/mysql.h"
#include <memory>
#include <string>
#include <unordered_map>

class DBResult
{
  public:
    DBResult() = default;
    ~DBResult() = default;

    explicit DBResult(std::shared_ptr<MYSQL_RES> result) {
        result_ = result;
        if (!result_) return;

        num_fields_ = mysql_num_fields(result_.get());
        MYSQL_FIELD *fields = mysql_fetch_fields(result_.get());
        for (unsigned int i = 0; i < num_fields_; i++) {
            field_index_map_.emplace(fields[i].name, i);
        }
    }

    unsigned int getNumFields() const {
        return num_fields_;
    }

    bool nextRow()
    {
        if (!result_)
            return false;
        row_ = mysql_fetch_row(result_.get());
        lengths_ = mysql_fetch_lengths(result_.get());
        return row_ != nullptr;
    }

    const char *getRaw(unsigned int index) const
    {
        if (!row_ || index >= num_fields_)
            return nullptr;
        return row_[index];
    }

    const char* getRaw(const std::string& field_name) const
    {
        auto it = field_index_map_.find(field_name);
        if (it == field_index_map_.end())
            return nullptr;
        return getRaw(it->second);
    }

  private:
    std::shared_ptr<MYSQL_RES> result_ = nullptr;
    MYSQL_ROW row_ = nullptr;
    unsigned int num_fields_ = 0;
    unsigned long *lengths_ = nullptr;
    std::unordered_map<std::string, int> field_index_map_;
};

#endif