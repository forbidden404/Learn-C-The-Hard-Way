#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    int max_data;
    int max_rows;
    struct Address *rows;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

struct Connection *conn;

void Database_close();
void Database_create(int, int);

void die(const char *message)
{
    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    Database_close();

    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load()
{
    int rc = fread(&conn->db->max_data, sizeof(int), 1, conn->file);
    if (rc != 1)
        die("Failed to load max_data.");

    rc = fread(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if (rc != 1)
        die("Failed to load max_rows.");

    conn->db->rows = malloc(sizeof(struct Address) * conn->db->max_rows);

    int i = 0;
    for (; i < conn->db->max_rows; i++) {
        struct Address *addr = &conn->db->rows[i];

        rc = fread(&addr->id, sizeof(int), 1, conn->file);
        if (rc != 1)
            die("Failed to load id.");

        rc = fread(&addr->set, sizeof(int), 1, conn->file);
        if (rc != 1)
            die("Failed to load set.");

        addr->name = malloc(conn->db->max_data);
        rc = fread(addr->name, conn->db->max_data, 1, conn->file);
        if (rc != 1)
            die("Failed to load name.");

        addr->email = malloc(conn->db->max_data);
        rc = fread(addr->email, conn->db->max_data, 1, conn->file);
        if (rc != 1)
            die("Failed to load email.");
    }
}

void Database_open(const char *filename, char mode)
{
    conn = malloc(sizeof(struct Connection));
    if (!conn)
        die("Memory error");

    conn->db = malloc(sizeof(struct Database));
    if (!conn->db)
        die("Memory error");

    if (mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if (conn->file) {
            Database_load();
        }
    }

    if (!conn->file)
        die("Failed to open the file");
}

void Database_close()
{
    if (conn) {
        if (conn->file)
            fclose(conn->file);
        if (conn->db)
            free(conn->db);
        free(conn);
    }
}

void Database_write()
{
    rewind(conn->file);

    int rc = fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
    if (rc != 1)
        die("Failed to write max_data.");

    rc = fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if (rc != 1)
        die("Failed to write max_rows.");

    int i = 0;
    for (; i < conn->db->max_rows; i++) {
        struct Address *addr = &conn->db->rows[i];

        rc = fwrite(&addr->id, sizeof(int), 1, conn->file);
        if (rc != 1)
            die("Failed to write id.");

        rc = fwrite(&addr->set, sizeof(int), 1, conn->file);
        if (rc != 1)
            die("Failed to write set.");

        rc = fwrite(addr->name, conn->db->max_data, 1, conn->file);
        if (rc != 1)
            die("Failed to write name.");

        rc = fwrite(addr->email, conn->db->max_data, 1, conn->file);
        if (rc != 1)
            die("Failed to write email.");
    }

    rc = fflush(conn->file);
    if (rc == -1)
        die("Cannot flush database.");
}

void Database_create(int max_rows, int max_data)
{
    int i = 0;

    conn->db->max_rows = max_rows;
    conn->db->max_data = max_data;
    conn->db->rows = malloc(sizeof(int) * 2 + max_data * 2);

    for (i = 0; i < conn->db->max_rows; i++) {
        struct Address addr = { .id = i, .set = 0 };

        addr.name = malloc(conn->db->max_data);
        addr.email = malloc(conn->db->max_data);
        conn->db->rows[i] = addr;
    }
}

void Database_set(int id, const char *name,
        const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if (addr->set)
        die("Already set, delete it first");

    addr->set = 1;

    char *res = strncpy(addr->name, name, conn->db->max_data);
    int len = min(strlen(name), conn->db->max_data - 1);
    addr->name[len] = '\0';
    if (!res)
        die("Name copy failed");

    res = strncpy(addr->email, email, conn->db->max_data);
    len = min(strlen(email), conn->db->max_data - 1);
    addr->email[len] = '\0';
    if (!res)
        die("Email copy failed");
}

void Database_get(int id)
{
    struct Address *addr = &conn->db->rows[id];

    if (addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set");
    }
}

void Database_delete(int id)
{
    struct Address addr = { .id = id, .set = 0 };
    addr.name = malloc(conn->db->max_data);
    addr.email = malloc(conn->db->max_data);
    conn->db->rows[id] = addr;
}

void Database_list()
{
    int i = 0;
    struct Database *db = conn->db;

    for (i = 0; i < conn->db->max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if (cur->set) {
            Address_print(cur);
        }
    }
}

void Database_find(const char *key, const char *value)
{
    int i = 0;
    for(; i < conn->db->max_rows; i++) {
        struct Address *addr = &conn->db->rows[i];

        int shouldPrintName = !strcmp(key, "name") && !strcmp(value, addr->name);
        int shouldPrintEmail = !strcmp(key, "email") && !strcmp(value, addr->email);
        if (shouldPrintName || shouldPrintEmail)
            Address_print(addr);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
        die("USAGE: ex17 <dbfile> <action> [action params]");

    char *filename = argv[1];
    char action = argv[2][0];
    Database_open(filename, action);

    int id = 0;

    switch (action) {
        case 'c':
            if (argc != 5)
                die("Need an max_rows, max_data for database.");
            int max_rows = atoi(argv[3]);
            int max_data = atoi(argv[4]);
            Database_create(max_rows, max_data);
            Database_write();
            break;

        case 'g':
            if (argc != 4)
                die("Need an id to get");

            id = atoi(argv[3]);
            if (id >= conn->db->max_rows) die("there's not that many records.");

            Database_get(id);
            break;

        case 's':
            if (argc != 6)
                die("Need id, name, email to set");

            id = atoi(argv[3]);
            if (id >= conn->db->max_rows) die("there's not that many records.");

            Database_set(id, argv[4], argv[5]);
            Database_write();
            break;

        case 'd':
            if (argc != 4)
                die("Need id to delete");

            id = atoi(argv[3]);
            if (id >= conn->db->max_rows) die("there's not that many records.");

            Database_delete(id);
            Database_write();
            break;

        case 'l':
            Database_list();
            break;

        case 'f':
            if (argc != 5)
                die("Need a key and value to search.");

            Database_find(argv[3], argv[4]);
            break;

        default:
            die("Invalid action: c=create, g=get, s=set, d=del, l=list, f=find");
    }

    Database_close();

    return 0;
}
