#ifndef SQL_PARSER_H
#define SQL_PARSER_H

#include <stdbool.h>

bool validate_sql_query(const char *query);
void extract_sql_keywords(const char *query);
void print_sql_error(const char *query);

#endif