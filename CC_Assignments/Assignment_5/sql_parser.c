#include "sql_parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

const char *sql_keywords[] = {
    "ADD", "ALL", "ALTER", "AND", "ANY", "AS", "ASC", "BACKUP", "BETWEEN", 
    "BY", "CASE", "CHECK", "COLUMN", "CONSTRAINT", "CREATE", "CROSS", 
    "DATABASE", "DEFAULT", "DELETE", "DESC", "DISTINCT", "DROP", "ELSE", 
    "EXISTS", "FOREIGN", "FROM", "FULL", "GROUP", "HAVING", "IN", "INDEX", 
    "INNER", "INSERT", "INTO", "IS", "JOIN", "KEY", "LEFT", "LIKE", "LIMIT", 
    "NOT", "NULL", "ON", "OR", "ORDER", "OUTER", "PRIMARY", "REFERENCES", 
    "RIGHT", "ROW", "SELECT", "SET", "TABLE", "TOP", "UNION", "UNIQUE", 
    "UPDATE", "VALUES", "VIEW", "WHERE", "WITH"
};

const int keyword_count = sizeof(sql_keywords) / sizeof(sql_keywords[0]);

bool validate_sql_query(const char *query) {
    if (strstr(query, ";") == NULL) {
        printf("Error: Query must end with a semicolon.\n");
        return false;
    }
    return true;
}

void extract_sql_keywords(const char *query) {
    printf("Extracted SQL Keywords:\n");
    char buffer[256];
    strcpy(buffer, query);

    char *token = strtok(buffer, " ,;");
    while (token != NULL) {
        for (int i = 0; i < keyword_count; i++) {
            if (strcasecmp(token, sql_keywords[i]) == 0) {
                printf(" - %s\n", token);
            }
        }
        token = strtok(NULL, " ,;");
    }
}

void print_sql_error(const char *query) {
    if (!validate_sql_query(query)) {
        printf("Invalid SQL syntax: %s\n", query);
    }
}