#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

const char* default_https_port_ = "443";
const int http_1_1_version_ = 11;

class Https: public std::enable_shared_from_this<Https> {
public:
    Https(boost::asio::any_io_executor executor);
    
    void DoGet(const std::string& host,
               const std::string& port,
               const std::string& path);

    const boost::beast::http::response<boost::beast::http::dynamic_body>& GetResponse() const {
        return response_;
    }

protected:
    // 域名解析完成回调
    void on_resolve(boost::beast::error_code error,
                    boost::asio::ip::tcp::resolver::results_type results);

    // (TCP)连接建立完成回调
    void on_connect(boost::beast::error_code error,
                    boost::asio::ip::tcp::resolver::results_type::endpoint_type);
    
    // SSL handlshake 完成回调
    void on_handshake(boost::beast::error_code error);

    // 发送数据完成回调
    void on_write(boost::beast::error_code error, std::size_t);

    // 接收数据完成回调
    void on_read(boost::beast::error_code error, std::size_t);

    // 关闭连接完成回调
    void on_shutdown(boost::beast::error_code error);

private:
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ssl::context ssl_context_;
    boost::asio::ssl::stream<boost::beast::tcp_stream> stream_;

    boost::beast::flat_buffer buffer_;
    boost::beast::http::request<boost::beast::http::empty_body> request_;
    boost::beast::http::response<boost::beast::http::dynamic_body> response_;
};

Https::Https(boost::asio::any_io_executor executor)
  : resolver_(executor),
    ssl_context_(boost::asio::ssl::context::tlsv12_client),
    stream_(executor, ssl_context_) {
    ssl_context_.set_verify_mode(boost::asio::ssl::verify_none);
}

void Https::DoGet(const std::string& host,
                  const std::string& port,
                  const std::string& path) {
    // 初始化请求参数
    request_.version(http_1_1_version_); // http 1.1
    request_.method(boost::beast::http::verb::get);
    request_.target(path);
    request_.set(boost::beast::http::field::host, host);
    request_.set(boost::beast::http::field::accept, "*/*");

    // 开始 域名解析
    // 之后的每一步都在上一步的完成回调中进行
    resolver_.async_resolve(host, port,
                            boost::beast::bind_front_handler(&Https::on_resolve, shared_from_this()));
}

void Https::on_resolve(boost::beast::error_code error,
                       boost::asio::ip::tcp::resolver::results_type results) {
    if (error) return;

    boost::beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(10));
    boost::beast::get_lowest_layer(stream_).async_connect(results,
        boost::beast::bind_front_handler(&Https::on_connect, shared_from_this()));
}

void Https::on_connect(boost::beast::error_code error,
                       boost::asio::ip::tcp::resolver::results_type::endpoint_type) {
    if (error) return;

    stream_.async_handshake(boost::asio::ssl::stream_base::client,
        boost::beast::bind_front_handler(&Https::on_handshake, shared_from_this()));
}

void Https::on_handshake(boost::beast::error_code error) {
    if (error) return;

    boost::beast::http::async_write(stream_, request_,
        boost::beast::bind_front_handler(&Https::on_write, shared_from_this()));
}

void Https::on_write(boost::beast::error_code error, std::size_t) {
    if (error) return;

    boost::beast::http::async_read(stream_, buffer_, response_,
        boost::beast::bind_front_handler(&Https::on_read, shared_from_this()));
}

void Https::on_read(boost::beast::error_code error, std::size_t) {
    if (error) return;

    stream_.async_shutdown(boost::beast::bind_front_handler(&Https::on_shutdown, shared_from_this()));
}

void Https::on_shutdown(boost::beast::error_code ) {
}

int GetUrlResult(const char* host, const char* path) {
    boost::asio::io_context context;

    std::shared_ptr<Https> https = std::make_shared<Https>(boost::asio::make_strand(context));
    https->DoGet(host, default_https_port_, path);

    // 类Https 使用的 boost::asio::any_io_executor 来自 context
    // 不同于 boost::asio::system_executor
    // 这里的类Https的几个回调函数是在 context.run() 中执行的
    // 所以，下面要明确执行 context.run()
    context.run();
    // 这种方式能指定回调执行的线程
    // 实际上, 这是更常见的方式

    // 这里偷懒了, 如果过程中错误，这里没有返回 error_code
    return https->GetResponse().result_int();
}

int main() {
    // 访问: https://www.baidu.com/
    int r = GetUrlResult("www.baidu.com", "/");

    std::cout << r << std::endl;

    return 0;
}