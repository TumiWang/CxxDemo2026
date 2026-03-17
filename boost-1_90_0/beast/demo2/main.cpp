#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/fiber/all.hpp>

class StatisticsDuration final {
public:
    StatisticsDuration(const std::string& prefix)
        : prefix_(prefix) {
        start_ = std::chrono::high_resolution_clock::now();
    }
    ~StatisticsDuration() {
        auto end = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> duration = end - start_;

        std::ostringstream os;
        os << prefix_ << " ---- " << duration.count() << "秒";

        std::cout << os.str() << std::endl;
    }

private:
    const std::string prefix_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

const char* default_https_port_ = "443";
const int http_1_1_version_ = 11;

int GetUrlResult(const char* host, const char* path) {

    boost::asio::io_context is_context;
    boost::asio::ip::tcp::resolver resolver(is_context);

    boost::asio::ip::tcp::resolver::results_type results = resolver.resolve(host, default_https_port_);
    if (results.empty()) return 0;

    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tlsv12_client);
    ssl_context.set_verify_mode(boost::asio::ssl::verify_none);

    boost::asio::ssl::stream<boost::beast::tcp_stream> stream(is_context, ssl_context);
    SSL_set_tlsext_host_name(stream.native_handle(), host);

    boost::beast::error_code error_code;

    boost::beast::get_lowest_layer(stream).connect(results, error_code);
    if (error_code) return error_code.value();

    stream.handshake(boost::asio::ssl::stream_base::client, error_code);
    if (error_code) return error_code.value();

    boost::beast::http::request<boost::beast::http::string_body> request{boost::beast::http::verb::get, path, http_1_1_version_};
    request.set(boost::beast::http::field::host, host);
    request.set(boost::beast::http::field::accept, "*/*");
    boost::beast::http::write(stream, request, error_code);
    if (error_code) return error_code.value();

    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::dynamic_body> response;
    boost::beast::http::read(stream, buffer, response, error_code);
    if (error_code) return error_code.value();

    stream.shutdown(error_code);
    if (error_code) return error_code.value();

    return response.result_int();
}

int main() {
    StatisticsDuration d("总耗时");

    std::vector<boost::fibers::fiber> fibers;

    fibers.emplace_back([]{
        // 访问: https://www.baidu.com/
        StatisticsDuration d("https://www.baidu.com/");
        GetUrlResult("www.baidu.com", "/");
    });

    fibers.emplace_back([]{
        // 访问: https://cmake.org/cmake/help/latest/index.html
        StatisticsDuration d("https://cmake.org/cmake/help/latest/index.html");
        GetUrlResult("cmake.org", "/cmake/help/latest/index.html");
    });

    for (auto& fiber: fibers) {
        fiber.join();
    }

    return 0;
}