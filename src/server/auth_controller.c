#include "server/auth_controller.h"
#include "server/utils.h"

void registerr()
{
    int rsize;
    user = btopn("../../../db/user.bt", 0, 0);
    btpos(user, ZSTART);
    if (!btsel(user, info1, info2, MAX, &rsize))
        server_make_protocol("NOKE", "Tài khoản đã tồn tại, vui lòng chọn tài khoản khác");
    else
    {
        if (!btins(user, info1, info2, MAX))
        {
            if ((user_dict = btcrt(make_dict_path(info1), 0, 0)) == NULL)
            {
                perror("Lỗi không thể tạo file từ điển cá nhân");
                return;
            }
            if ((user_note = btcrt(make_note_path(info1), 0, 0)) == NULL)
            {
                perror("Lỗi không thể tạo file ghi chú");
                return;
            }
            if (fclose(fopen(make_his_path(info1), "w")) != 0)
            {
                perror("Lỗi không thể tạo file lịch sử tra cứu");
                return -1;
            }
            if (fclose(fopen(make_game_his_path(info1), "w")) != 0)
            {
                perror("Lỗi không thể tạo file lịch sử luyện tập");
                return -1;
            }
            server_make_protocol("OKE", NULL);
        }
        else
            server_make_protocol("NOKE", "Đăng ký thất bại, chương trình lỗi...");
    }
    btcls(user);
    btcls(user_dict);
    btcls(user_note);
}

void login()
{
    char *pass = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    user = btopn("../../../db/user.bt", 0, 0);
    btpos(user, ZSTART);

    if (btsel(user, info1, pass, MAX, &rsize))
        server_make_protocol("NOKE", "Tài khoản không tồn tại");
    else
    {
        if (strcmp(pass, info2) != 0)
            server_make_protocol("NOKE", "Mật khẩu không đúng");
        else
        {
            strcpy(username, info1);
            server_make_protocol("OKE", NULL);
        }
    }
    btcls(user);
}

void change_pass() {
    char *pass = (char *)malloc(sizeof(char) * MAX);
    int rsize;
    user = btopn("../../../db/user.bt", 0, 0);
    btpos(user, ZSTART);

    if (btsel(user, username, pass, MAX, &rsize))
        server_make_protocol("NOKE", "Tài khoản không tồn tại");
    else
    {
        if (strcmp(pass, info1) != 0)
            server_make_protocol("NOKE", "Mật khẩu không đúng");
        else
        {
            btupd(user, username, info2, MAX);
            server_make_protocol("OKE", NULL);
        }
    }
    btcls(user);
}