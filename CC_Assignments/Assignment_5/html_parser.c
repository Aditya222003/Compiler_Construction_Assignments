#include "html_parser.h"
#include <stdio.h>
#include <string.h>

void parse_html(const char *html) {
    printf("Extracted HTML Tags:\n");
    const char *ptr = html;

    while ((ptr = strchr(ptr, '<')) != NULL) {
        const char *end = strchr(ptr, '>');
        if (end) {
            char tag[128] = {0};
            strncpy(tag, ptr + 1, end - ptr - 1);
            printf(" - %s\n", tag);
            ptr = end + 1;
        } else {
            break;
        }
    }
}

void check_html_syntax(const char *html) {
    if (strstr(html, "<html>") && strstr(html, "</html>")) {
        printf("HTML syntax is valid.\n");
    } else {
        printf("HTML syntax error: Missing <html> or </html> tag.\n");
    }
}