#ifndef SERVER_ID_H
#define SERVER_ID_H

int get_lineindex_from_lineid(int i);

int get_lineid_from_lineindex(int i);

int insert_id(int id);

int delete_id(int id);

void initialize_id();

void print_ids();

int get_ids(char* c);

#endif