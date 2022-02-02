#include "server/main_controller.h"
#include "server/utils.h"

void scan_dict(BTA *b)
{
    int rsize, k = 0, count = 0;

    char *eng = (char *)malloc(sizeof(char) * MAX);
    char *vie = (char *)malloc(sizeof(char) * MAX);
    while (!btseln(b, eng, vie, MAX, &rsize))
    {
        if (eng[0] != info1[0])
            break;
        for (int i = 0; i < strlen(info1); i++)
        {
            if (eng[i] != info1[i])
            {
                k = 1;
                break;
            }
        }
        if (k == 0)
        {
            count++;
            strcat(suggest_protocol_str, "|");
            strcat(suggest_protocol_str, eng);
        }
        k = 0;
        if (count > 20)
            break;
    }
    free(eng);
    free(vie);
}

void suggest_query(BTA *b)
{
    char kd[2];
    int rsize;
    char *vie = (char *)malloc(sizeof(char) * MAX);

    if (b == dict)
    {
        kd[0] = info1[0];
        kd[1] = '\0';

        if (!btsel(b, kd, vie, sizeof(char *), &rsize))
        {
            scan_dict(b);
        }
    }
    else
    {
        btpos(b, ZSTART);
        scan_dict(b);
    }
    free(vie);
}

void suggestion()
{
    dict = btopn("../../../db/dict.bt", 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    strcpy(suggest_protocol_str, "ENG");
    suggest_query(user_dict);
    suggest_query(dict);
    puts(suggest_protocol_str);
    send(connfd, suggest_protocol_str, MAX, 0);
    btcls(dict);
    btcls(user_dict);
}

void get_history()
{
    char buffer[MAX];
    char line[MAX];
    if ((f = fopen(make_his_path(username), "r")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    while (fgets(line, MAX, f))
    {
        memset(buffer, 0, 4);
        strcpy(buffer, line);
        strcat(buffer, his);
        strcpy(his, buffer);
    }
    strcat(his, "\n");
    if (strlen(his) == 0)
        server_make_protocol("NOKE", "Không tìm thấy lịch sử tra cứu");
    else
        server_make_protocol("OKE", his);
    fclose(f);
}

void del_his()
{
    int i;
    if ((i = fclose(fopen(make_his_path(username), "w"))) != 0)
        send(connfd, "NOKE", MAX, 0);
    else
        send(connfd, "OKE", MAX, 0);
    strcpy(his, "");
}

void new_history_handle(char *text)
{
    char *buffer = (char *)malloc(sizeof(char) * MAX);
    char *buftrans = (char *)malloc(sizeof(char) * MAX);
    sprintf(buftrans, "%s\n", text);
    int i = strremove(his, buftrans);
    strcpy(buffer, buftrans);
    strcat(buffer, his);
    strcpy(his, buffer);
    free(buffer);
    if (i == 0)
        add_to_history(text);
    else
        rewrite_history();
}

void rewrite_history()
{
}

int strremove(char *str, char *sub)
{
    size_t len = strlen(sub);
    if (len > 0)
    {
        char *p = str;
        if ((p = strstr(p, sub)) != NULL)
        {
            memmove(p, p + len, strlen(p + len) + 1);
            return 1;
        }
    }
    return 0;
}

void add_to_history(char *buf)
{
    char line[MAX];
    if ((f = fopen(make_his_path(username), "a")) == NULL)
    {
        printf("Lỗi không thể mở file.\n");
        return -1;
    }
    fprintf(f, "%s\n", buf);
    fclose(f);
}

void translate()
{
    char *value = (char *)malloc(sizeof(char) * MAX);
    char *edited_mean = (char *)malloc(sizeof(char) * MAX);
    char *origin_mean = (char *)malloc(sizeof(char) * MAX);
    dict = btopn("../../../db/dict.bt", 0, 0);
    user_dict = btopn(make_dict_path(username), 0, 0);
    btpos(dict, ZSTART);
    btpos(user_dict, ZSTART);
    int rsize;
    strcpy(edited_mean, "");
    strcpy(origin_mean, "");

    if (!btsel(user_dict, info1, value, MAX, &rsize))
        strcpy(edited_mean, value);
    if (!btsel(dict, info1, value, MAX, &rsize))
        strcpy(origin_mean, value);
    if (strlen(edited_mean) == 0 && strlen(origin_mean) == 0)
        server_make_protocol("NOKE", "Không tìm thấy từ");
    else
    {
        make_protocol_two_msgs("VIE", edited_mean, origin_mean);
        new_history_handle(info1);
    }

    free(value);
    free(edited_mean);
    free(origin_mean);
    btcls(dict);
    btcls(user_dict);
}
