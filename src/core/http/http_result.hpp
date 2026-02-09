#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

struct HttpResponse
{
    enum class Code : int
    {
        SUCCESS = 200,
        CREATED = 201,
        NO_CONTENT = 204,

        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,

        INTERNAL_SERVER_ERROR = 500,
    };

    int code;
    std::string msg;
    json data;

    static HttpResponse success(json data = json::object(), std::string msg = "success")
    {
        return HttpResponse{200, msg, data};
    }

    static HttpResponse error(int code, std::string msg, json data = json::object())
    {
        return HttpResponse{code, msg, data};
    }

    std::string dump() const
    {
        json j;
        j["code"] = code;
        j["msg"] = msg;
        if (!data.is_null())
        {
            j["data"] = data;
        }
        else 
        {
            j["data"] = json::object();
        }
        return j.dump();
    }
};
