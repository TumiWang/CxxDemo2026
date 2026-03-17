#include <iostream>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

const char* default_http_port_ = "80";
const int http_1_1_version_ = 11;

int GetUrlResult(const char* host, const char* path) {
    boost::beast::error_code error_code;
    boost::asio::io_context is_context;
    boost::asio::ip::tcp::resolver resolver(is_context);

    // 下面是 建立https连接 的第一步 解析域名等
    boost::asio::ip::tcp::resolver::results_type results = resolver.resolve(host, default_http_port_);
    if (results.empty()) return 0;

    // https 使用的是 boost::asio::ssl::stream<boost::beast::tcp_stream>
    boost::beast::tcp_stream stream(is_context);

    // 下面是 建立https连接 的第二步 建立连接
    stream.connect(results, error_code);
    if (error_code) return error_code.value();

    // 下面是 建立https连接 的第三步 发送数据
    boost::beast::http::request<boost::beast::http::string_body> request{boost::beast::http::verb::get, path, http_1_1_version_};
    request.set(boost::beast::http::field::host, host);
    request.set(boost::beast::http::field::accept, "*/*");
    boost::beast::http::write(stream, request, error_code);
    if (error_code) return error_code.value();

    // 下面是 建立https连接 的第四步 接收数据
    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::dynamic_body> response;
    boost::beast::http::read(stream, buffer, response, error_code);
    if (error_code) return error_code.value();

    // 下面是 建立https连接 的第五步 关闭连接
    stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error_code);
    if (error_code) return error_code.value();

    // 下面是打印的 所有返回的内容
    std::cout << response << std::endl;

    // 正常返回值应该为 200
    // 当然，也可能返回 302 等
    return response.result_int();
}

int main() {
    // 访问: https://www.baidu.com/
    int r = GetUrlResult("www.baidu.com", "/");

    std::cout << r << std::endl;

    return 0;
}