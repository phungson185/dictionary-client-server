#include "client/auth_controller.h"

int main_handler(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    gdk_color_parse("red", &red);
    gdk_color_parse("green", &green);

    builder = gtk_builder_new_from_file("../../../ui/dict-app.glade");
    window_login = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));

    gtk_builder_connect_signals(builder, NULL);

    log_acc = GTK_WIDGET(gtk_builder_get_object(builder, "log_acc"));
    log_pass = GTK_WIDGET(gtk_builder_get_object(builder, "log_pass"));
    log_noti = GTK_WIDGET(gtk_builder_get_object(builder, "log_noti"));

    g_signal_connect(window_login, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_object_unref(builder);
    gtk_widget_show(window_login);
    gtk_main();

    return 0;
}

void show_register()
{

    builder = gtk_builder_new_from_file("../../../ui/dict-app.glade");

    window_register = GTK_WIDGET(gtk_builder_get_object(builder, "window_register"));
    gtk_builder_connect_signals(builder, NULL);

    reg_acc = GTK_WIDGET(gtk_builder_get_object(builder, "reg_acc"));
    reg_pass = GTK_WIDGET(gtk_builder_get_object(builder, "reg_pass"));
    retype_pass = GTK_WIDGET(gtk_builder_get_object(builder, "retype_pass"));
    reg_noti = GTK_WIDGET(gtk_builder_get_object(builder, "reg_noti"));

    g_object_unref(builder);
    gtk_widget_show(window_register);
}

void registerr()
{
    char *repass = (char *)malloc(sizeof(char) * MAX);
    strcpy(info1, gtk_entry_get_text(GTK_ENTRY(reg_acc)));
    strcpy(info2, gtk_entry_get_text(GTK_ENTRY(reg_pass)));
    strcpy(repass, gtk_entry_get_text(GTK_ENTRY(retype_pass)));

    if (strcmp(info1, "") == 0 || strcmp(info2, "") == 0 || strcmp(repass, "") == 0)
        show_message(window_register, GTK_MESSAGE_ERROR, "ERROR!", "Thông tin đăng ký còn thiếu");
    else if (strcmp(info2, repass) != 0)
        show_message(window_register, GTK_MESSAGE_ERROR, "ERROR!", "Mật khẩu không khớp");
    else
    {
        make_protocol("REG", info1, info2);
        if (strcmp(key, "OKE") == 0)
        {
            gtk_widget_hide(window_register);
            show_message(window_login, GTK_MESSAGE_INFO, "SUCCESS!", "Đăng ký thành công");
            gtk_entry_set_text(GTK_ENTRY(log_acc), info1);
        }
        else if (strcmp(key, "NOKE") == 0)
            show_message(window_register, GTK_MESSAGE_ERROR, "ERROR!", info1);
    }
    free(repass);
}

void login()
{
    strcpy(info1, gtk_entry_get_text(GTK_ENTRY(log_acc)));
    strcpy(info2, gtk_entry_get_text(GTK_ENTRY(log_pass)));
    if (strcmp(info1, "") == 0 || strcmp(info2, "") == 0)
        show_message(window_login, GTK_MESSAGE_ERROR, "ERROR!", "Thông tin đăng nhập còn thiếu");
    else
    {
        make_protocol("LOG", info1, info2);
        if (strcmp(key, "OKE") == 0)
        {
            strcpy(user, info1);
            show_main_window();
        }
        else if (strcmp(key, "NOKE") == 0)
            show_message(window_login, GTK_MESSAGE_ERROR, "ERROR!", info1);
    }
}

void open_profile()
{
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../../../ui/dict-app.glade");

    window_profile = GTK_WIDGET(gtk_builder_get_object(builder, "window_profile"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show(window_about);
    gtk_widget_show(window_profile);
}

void change_pass()
{
    gtk_widget_destroy(window_profile);

    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("../../../ui/dict-app.glade");

    window_change_pass = GTK_WIDGET(gtk_builder_get_object(builder, "window_change_pass"));
    gtk_builder_connect_signals(builder, NULL);

    old_pass = GTK_WIDGET(gtk_builder_get_object(builder, "old_pass"));
    new_pass = GTK_WIDGET(gtk_builder_get_object(builder, "new_pass"));
    retype_new_pass = GTK_WIDGET(gtk_builder_get_object(builder, "retype_new_pass"));

    g_object_unref(builder);
    gtk_widget_show(window_change_pass);
}

void exec_change_pass()
{
    char *repass = (char *)malloc(sizeof(char) * MAX);
    strcpy(info1, gtk_entry_get_text(GTK_ENTRY(old_pass)));
    strcpy(info2, gtk_entry_get_text(GTK_ENTRY(new_pass)));
    strcpy(repass, gtk_entry_get_text(GTK_ENTRY(retype_new_pass)));

    if (strcmp(info1, "") == 0 || strcmp(info2, "") == 0 || strcmp(repass, "") == 0)
        show_message(window_change_pass, GTK_MESSAGE_ERROR, "ERROR!", "Thông tin đổi mật khẩu còn thiếu");
    else if (strcmp(info2, repass) != 0)
        show_message(window_change_pass, GTK_MESSAGE_ERROR, "ERROR!", "Mật khẩu không khớp");
    else
    {
        make_protocol("CPASS", info1, info2);
        if (strcmp(key, "OKE") == 0)
        {
            show_message(window_change_pass, GTK_MESSAGE_INFO, "SUCCESS!", "Đổi mật khẩu thành công");
            gtk_widget_destroy(window_change_pass);
            gtk_widget_show(window_main);
        }
        else if (strcmp(key, "NOKE") == 0)
            show_message(window_change_pass, GTK_MESSAGE_ERROR, "ERROR!", info1);
    }
    free(repass);
}

void logout()
{
    make_protocol("LOGOUT", NULL, NULL);
    if (strcmp(key, "OKE") == 0)
    {
        gtk_widget_destroy(window_profile);
        gtk_widget_hide(window_main);
        gtk_widget_show(window_login);
    }
}
