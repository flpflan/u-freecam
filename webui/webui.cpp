#include "webui.hpp"
#include "config.hpp"
#include "json.hpp"
#include "page.inc"

#include "httplib.h"

#include <string>

namespace
{
    static httplib::Server svr;
    static void page(const httplib::Request &, httplib::Response &res)
    {
        res.set_header("Content-Encoding", "gzip");
        res.set_content(reinterpret_cast<const char *>(__webui_min_html_gz), __webui_min_html_gz_len, "text/html");
    }

    static void getConfig(const httplib::Request &, httplib::Response &res)
    {
        res.set_content(user_config::to_json(), "application/json");
    }
    static void setConfig(const httplib::Request &req, httplib::Response &res)
    {
        if (req.get_header_value("Content-Type") == "application/json")
            if (const auto data = json::parse(req.body)) user_config::from_json(*data);
        return res.set_content(user_config::to_json(), "application/json");
    }
    static void schema(const httplib::Request &, httplib::Response &res)
    {
        res.set_header("Content-Encoding", "gzip");
        res.set_content(user_config::schema(), "application/json");
    }
}

namespace webui
{

    void start()
    {
        svr.Get("/", page);
        svr.Get("/config/schema", schema);
        svr.Get("/config", getConfig);
        svr.Post("/config", setConfig);

        svr.listen("0.0.0.0", user_config::webui::Port);
    }
    void stop() { svr.stop(); }
}
