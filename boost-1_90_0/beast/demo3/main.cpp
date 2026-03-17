#include <iostream>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/fiber/all.hpp>

const char* default_https_port_ = "443";
const int http_1_1_version_ = 11;

int GetUrlResult(const char* host, const char* path) {
    boost::asio::system_executor executor;
    boost::asio::ip::tcp::resolver resolver(executor);

    boost::fibers::promise<boost::asio::ip::tcp::resolver::results_type> resolver_promise;
    boost::fibers::future<boost::asio::ip::tcp::resolver::results_type> resolver_future(resolver_promise.get_future());

    resolver.async_resolve(host, default_https_port_,
        [&](boost::beast::error_code, boost::asio::ip::tcp::resolver::results_type results) {
            resolver_promise.set_value(results);
        });
    boost::asio::ip::tcp::resolver::results_type results = resolver_future.get();
    if (results.empty()) return 0;

    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tlsv12_client);
    ssl_context.set_verify_mode(boost::asio::ssl::verify_none);

    boost::asio::ssl::stream<boost::beast::tcp_stream> stream(executor, ssl_context);
    SSL_set_tlsext_host_name(stream.native_handle(), host);

    boost::beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(10));
    boost::beast::error_code error_code;

    boost::fibers::promise<boost::beast::error_code> connect_promise;
    boost::fibers::future<boost::beast::error_code> connect_future(connect_promise.get_future());
    boost::beast::get_lowest_layer(stream).async_connect(results,
        [&](boost::beast::error_code error, boost::asio::ip::tcp::resolver::results_type::endpoint_type) {
            connect_promise.set_value(error);
        });
    error_code = connect_future.get();
    if (error_code) return error_code.value();

    boost::fibers::promise<boost::beast::error_code> handshake_promise;
    boost::fibers::future<boost::beast::error_code> handshake_future(handshake_promise.get_future());
    stream.async_handshake(boost::asio::ssl::stream_base::client,
        [&](boost::beast::error_code error) {
            handshake_promise.set_value(error);
        });
    error_code = handshake_future.get();
    if (error_code) return error_code.value();

    boost::beast::http::request<boost::beast::http::string_body> request{boost::beast::http::verb::get, path, http_1_1_version_};
    request.set(boost::beast::http::field::host, host);
    request.set(boost::beast::http::field::accept, "*/*");
    boost::fibers::promise<boost::beast::error_code> write_promise;
    boost::fibers::future<boost::beast::error_code> write_future(write_promise.get_future());
    boost::beast::http::async_write(stream, request,
        [&](boost::beast::error_code error, std::size_t) {
            write_promise.set_value(error);
        });
    error_code = write_future.get();
    if (error_code) return error_code.value();

    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::dynamic_body> response;
    boost::fibers::promise<boost::beast::error_code> read_promise;
    boost::fibers::future<boost::beast::error_code> read_future(read_promise.get_future());

    boost::beast::http::async_read(stream, buffer, response,
        [&](boost::beast::error_code error, std::size_t) {
            read_promise.set_value(error);
        });
    error_code = read_future.get();
    if (error_code) return error_code.value();

    boost::fibers::promise<boost::beast::error_code> shutdown_promise;
    boost::fibers::future<boost::beast::error_code> shutdown_future(shutdown_promise.get_future());
    stream.async_shutdown([&](boost::beast::error_code error) {
            shutdown_promise.set_value(error);
        });
    error_code = shutdown_future.get();
    if (error_code) return error_code.value();

    return response.result_int();
}

int main() {
    int r = GetUrlResult("www.baidu.com", "/");

    std::cout << r << std::endl;

    return 0;
}