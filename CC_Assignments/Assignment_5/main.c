#include <stdio.h>
#include "sql_parser.h"
#include "html_parser.h"

int main() {
    const char *sql_query = "SELECT * FROM users INNER JOIN user_accounts  users.id = user_accounts.id WHERE id = 1 GROUP BY id ORDER BY name ASC; ";
    const char *html_code = "<html><body><h1>Hello, World!</h1></body></html>";

    printf("SQL Parser:\n");
    extract_sql_keywords(sql_query);
    print_sql_error(sql_query);
    
    printf("\nHTML Parser:\n");
    parse_html(html_code);
    check_html_syntax(html_code);    
    
    return 0;
}