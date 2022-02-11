#include "server/extend_controller.h"
#include "server/utils.h"

void add_dict()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    dict = btopn("../../../db/dict.bt", 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    btpos(dict, ZSTART);
    btpos(user_dict, ZSTART);
    if (!btsel(user_dict, info1, value, MAX, &rsize) || !btsel(dict, info1, value, MAX, &rsize))
        server_make_protocol("NOKE", "Từ đã tồn tại trong từ điển");
    else
    {
        if (!btins(user_dict, info1, info2, MAX))
            server_make_protocol("OKE", NULL);
        else
            server_make_protocol("NOKE", "Thêm từ thất bại");
    }
    free(value);
    btcls(dict);
    btcls(user_dict);
}

void edit_dict()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    dict = btopn("../../../db/dict.bt", 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    user_note = btopn(make_note_path(username), 0, 0);
    if (!btsel(user_dict, info1, value, MAX, &rsize))
    {
        if (!btsel(user_note, info1, value, MAX, &rsize))
        {
            if (!btupd(user_note, info1, info2, MAX))
                server_make_protocol("OKE", NULL);
            else
                server_make_protocol("NOKE", "Sửa từ thất bại");
        }
        
        if (!btupd(user_dict, info1, info2, MAX))
            server_make_protocol("OKE", NULL);
        else
            server_make_protocol("NOKE", "Sửa từ thất bại");
    }
    else
    {
        if (!btsel(dict, info1, value, MAX, &rsize))
        {
            if (!btins(user_dict, info1, info2, MAX))
                server_make_protocol("OKE", NULL);
            else
                server_make_protocol("NOKE", "Sửa từ thất bại");
        }
        else
            server_make_protocol("NOKE", "Từ bạn muốn chỉnh sửa không có trong từ điển");
    }
    free(value);
    btcls(dict);
    btcls(user_dict);
    btcls(user_note);
}

void del_dict()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    note = btopn(make_note_path(username), 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    btpos(user_dict, ZSTART);
    if (!btsel(user_dict, info1, value, MAX, &rsize))
    {
        if (!btdel(user_dict, info1))
        {
            if (!btsel(note, info1, value, MAX, &rsize))
                btdel(note, info1);
            server_make_protocol("OKE", NULL);
        }
        else
            server_make_protocol("NOKE", "Xóa từ thất bại");
    }
    else
        server_make_protocol("NOKE", "Từ bạn muốn xóa không có trong từ điển cá nhân");
    btcls(note);
    free(value);
    btcls(user_dict);
}
