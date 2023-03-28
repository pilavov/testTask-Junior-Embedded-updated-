//
// Created by kpilavov on 28.03.23.
//

#ifndef TESTTASK_OM_T3_JUNIOR_EMBEDDED_DATABASE_H
#define TESTTASK_OM_T3_JUNIOR_EMBEDDED_DATABASE_H

#include <iostream>
#include <vector>
#include <sqlite3.h>

using TwoDimVector = std::vector<std::vector<std::string>>;

class DataBase {
private:
    static const char m_charset[];
    const int m_rows = 1024;
    const int m_columns = 6;
    const int m_symbols_quantity =  8;
    const char m_separator = ',';

    std::string generate_random_string() const;
    bool is_vowel(char c) const;
    TwoDimVector read_csv(const std::string &input_filename) const;
    void edit_data_in_file(TwoDimVector& data) const;
    void save_edited_file(const TwoDimVector& data, const std::string& output_file) const;
    void edit_database(sqlite3 *db) const;
    void check_possible_sql_error(int rc, const char* zErrMsg, sqlite3* db) const;

public:
    DataBase();
    void fill_file(const std::string& filename) const;

    void edit_file(const std::string &input_filename, const std::string &output_filename) const;
    void create_sql_from_csv(const std::string& filename) const;
};


#endif //TESTTASK_OM_T3_JUNIOR_EMBEDDED_DATABASE_H
