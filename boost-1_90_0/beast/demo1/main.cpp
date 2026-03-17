#include <iostream>

#include <boost/asio.hpp>

// 下面这个隐式需要 OpenSSL 我这里使用的是 3.5.4
// 所以, 编译时需要链接 OpenSSL
#include <boost/asio/ssl.hpp>

#include <boost/beast.hpp>

const char* default_https_port_ = "443";
const int http_1_1_version_ = 11;

int GetUrlResult(const char* host, const char* path) {
    boost::beast::error_code error_code;
    boost::asio::io_context is_context;
    boost::asio::ip::tcp::resolver resolver(is_context);

    // 下面是 建立https连接 的第一步 解析域名等
    boost::asio::ip::tcp::resolver::results_type results = resolver.resolve(host, default_https_port_);
    if (results.empty()) return 0;

    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tlsv12_client);
    ssl_context.set_verify_mode(boost::asio::ssl::verify_none);

    boost::asio::ssl::stream<boost::beast::tcp_stream> stream(is_context, ssl_context);
    // SSL_set_tlsext_host_name 是 OpenSSL 库的函数
    SSL_set_tlsext_host_name(stream.native_handle(), host);

    // 下面是 建立https连接 的第二步 建立连接
    // boost::beast::get_lowest_layer(stream) 返回的是 boost::beast::tcp_stream 类型
    boost::beast::get_lowest_layer(stream).connect(results, error_code);
    if (error_code) return error_code.value();

    // 下面是 建立https连接 的第三步 SSL handshake
    stream.handshake(boost::asio::ssl::stream_base::client, error_code);
    if (error_code) return error_code.value();

    // 下面是 建立https连接 的第四步 发送数据
    // 第一个参数表示get 请求
    // 第二个参数表示访问的url的path部分
    // 第三个参数 http的版本为 1.1
    boost::beast::http::request<boost::beast::http::string_body> request{boost::beast::http::verb::get, path, http_1_1_version_};
    // 可以通过下面方法设置 host user-agent 等
    request.set(boost::beast::http::field::host, host);
    request.set(boost::beast::http::field::accept, "*/*");
    // request.set(boost::beast::http::field::user_agent, "...");
    boost::beast::http::write(stream, request, error_code);
    if (error_code) return error_code.value();

    // 下面是 建立https连接 的第五步 接收数据
    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::dynamic_body> response;
    boost::beast::http::read(stream, buffer, response, error_code);
    if (error_code) return error_code.value();

    // 下面是 建立https连接 的第六步 关闭连接
    stream.shutdown(error_code);
    if (error_code) return error_code.value();

    // 下面是打印的 所有返回的内容
    // std::cout << response << std::endl;

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