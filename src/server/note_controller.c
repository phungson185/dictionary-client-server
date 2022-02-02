#include "server/note_controller.h"
#include "server/utils.h"

void add_note()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    dict = btopn("../../../db/dict.bt", 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    user_note = btopn(make_note_path(username), 0, 0);

    if (!btsel(user_note, info1, value, MAX, &rsize))
        server_make_protocol("NOKE", "Từ này đã có trong danh sách ghi chú");
    else
    {
        if (!btsel(user_dict, info1, value, MAX, &rsize))
        {
            if (!btins(user_note, info1, value, MAX))
                server_make_protocol("OKE", NULL);
            else
                server_make_protocol("NOKE", "Thêm ghi chú thất bại");
        }
        else if (!btsel(dict, info1, value, MAX, &rsize))
        {
            if (!btins(user_note, info1, value, MAX))
                server_make_protocol("OKE", NULL);
            else
                server_make_protocol("NOKE", "Thêm ghi chú thất bại");
        }
        else
            server_make_protocol("NOKE", "Từ bạn muốn thêm ghi chú không có trong từ điển");
    }
    free(value);
    btcls(dict);
    btcls(user_dict);
    btcls(user_note);
}

void del_note()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    user_note = btopn(make_note_path(username), 0, 0);
    if (!btsel(user_note, info1, value, MAX, &rsize))
    {
        if (!btdel(user_note, info1))
            server_make_protocol("OKE", NULL);
        else
            server_make_protocol("NOKE", "Xóa ghi chú thất bại");
    }
    else
        server_make_protocol("NOKE", "Từ bạn muốn xóa không có trong danh sách ghi chú");
    free(value);
    btcls(user_note);
}

void del_all_note()
{
    if ((user_note = btcrt(make_note_path(info1), 0, 0)) == NULL)
        server_make_protocol("NOKE", "Xóa tất cả ghi chú thất bại");
    else
        server_make_protocol("OKE", NULL);
    btcls(user_note);
}

void get_note()
{
    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    int rsize, isEmpty = 1;
    user_note = btopn(make_note_path(username), 0, 0);
    btpos(user_note, ZSTART);
    strcpy(get_note_protol_str, "OKE");
    while (!btseln(user_note, eng, vie, MAX, &rsize))
    {
        isEmpty = 0;
        strcat(get_note_protol_str, "|");
        strcat(get_note_protol_str, eng);
    }
    if (isEmpty)
        server_make_protocol("NOKE", "Danh sách ghi chú rỗng");
    else
        send(connfd, get_note_protol_str, MAX, 0);
    free(eng);
    free(vie);
    btcls(user_note);
}