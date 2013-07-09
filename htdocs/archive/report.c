#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "webarchive.h"

#define MAX_RECORDS 1000

void listAll(char **data)
{
    puts("Content-type: text/html\r\n\r\n");
    puts("<html>");
    puts("<head>");
    puts("<meta http-equiv=\"Pragma\" content=\"no-cache\">");
    puts("<script>");
    puts("function addfile() {");
    puts("    var name = prompt(\"Name of library/platform/etc\", \"\");");
    puts("    if (name != null)");
    puts("        window.location = \"http://cppcheck.sf.net/cgi-bin/addfile.cgi?name=\" + name;");
    puts("}");
    puts("function editfile(name) {\n");
    puts("    window.location = \"http://cppcheck.sf.net/cgi-bin/edit.cgi?name=\" + name;\n");
    puts("}");
    puts("function renamefile(name1) {\n");
    puts("    var name2 = prompt(\"Name\", name1);\n");
    puts("    if (name2 != null)\n");
    puts("        window.location = \"http://cppcheck.sf.net/cgi-bin/renamefile.cgi?name1=\" + name1 + \"&name2=\" + name2;\n");
    puts("}\n");
    puts("function deletefile(name) {\n");
    puts("    window.location = \"http://cppcheck.sf.net/cgi-bin/deletefile.cgi?name=\" + name;\n");
    puts("}");
    puts("</script>");
    puts("</head><body>");
    puts("<input type=\"button\" onclick=\"addfile()\" value=\"Add file\">");
    puts("<table border=\"1\"><tr><td><table>");
    for (int i = 0; i < MAX_RECORDS && data[i]; i++) {
        const char *name = getname(data[i]);
        printf("<tr><td>%s</td>", name);
        printf("<td><input type=\"button\" onclick=\"editfile(\'%s\')\" value=\"Edit\"></td>", name);
        printf("<td><input type=\"button\" onclick=\"renamefile(\'%s\')\" value=\"Rename\"></td>", name);
        printf("<td><input type=\"button\" onclick=\"deletefile(\'%s\')\" value=\"Delete\"></td>", name);
        printf("</tr>\n");
    }
    puts("</table></td></tr></table>");
    puts("</body></html>");
}

void listOne(char **data, const char name[])
{
    int index = -1;
    for (int i = 0; i < MAX_RECORDS && data[i]; i++) {
        if (strcmp(getname(data[i]), name)==0) {
            index = i;
            break;
        }
    }

    puts("Content-type: text/plain\r\n\r\n");
    puts((index == -1) ? "Not found" : data[index]);
}

int main()
{
    char *data[MAX_RECORDS] = {0};

    // read
    if (!readdata(data, MAX_RECORDS)) {
        puts("Content-type: text/html\r\n\r\n");
        puts("Internal error: failed to load data");
        return 0;
    }

    // sort
    sortdata(data,MAX_RECORDS);

    const char *query_string = getenv("QUERY_STRING");
    if (query_string == NULL || *query_string == '\0') {
        listAll(data);
    } else if (strncmp(query_string, "name=", 5) == 0 && getname(query_string) != NULL) {
        char name[32] = {0};
        strcpy(name, getname(query_string));
        listOne(data,name);
    } else {
        puts("Content-type: text/plain\r\n\r\n");
        puts("Invalid query");
    }

    return 0;
}
