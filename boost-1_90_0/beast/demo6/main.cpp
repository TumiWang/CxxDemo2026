#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

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
    void on_resolve(boost::beast::error_code error,
                    boost::asio::ip::tcp::resolver::results_type results);
    void on_connect(boost::beast::error_code error,
                    boost::asio::ip::tcp::resolver::results_type::endpoint_type);
    void on_handshake(boost::beast::error_code error);
    void on_write(boost::beast::error_code error, std::size_t);
    void on_read(boost::beast::error_code error, std::size_t);
    void on_shutdown(boost::beast::error_code error);

private:
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ssl::context ssl_context_;
    boost::asio::ssl::stream<boost::beast::tcp_stream> stream_;

    boost::beast::flat_buffer buffer_;
    boost::beast::http::request<boost::beast::http::empty_body> request_;
    boost::beast::http::response<boost::beast::http::dynamic_body> response_;

    std::unique_ptr<StatisticsDuration> duration_;
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
    if (!duration_) {
        std::ostringstream os;
        os << "https://" << host;
        if (port != default_https_port_) {
            os << ":" << default_https_port_;
        }
        os << path;
        duration_.reset(new StatisticsDuration(os.str()));
    }

    request_.version(http_1_1_version_);
    request_.method(boost::beast::http::verb::get);
    request_.target(path);
    request_.set(boost::beast::http::field::host, host);
    request_.set(boost::beast::http::field::accept, "*/*");

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

    context.run();

    return https->GetResponse().result_int();
}

int main()
{
    StatisticsDuration d("总耗时");

    std::vector<std::thread> threads;

    threads.emplace_back([&]{
        // 访问: https://www.baidu.com/
        // StatisticsDuration d("https://www.baidu.com/");
        GetUrlResult("www.baidu.com", "/");
    });

    threads.emplace_back([&]{
        // 访问: https://cmake.org/cmake/help/latest/index.html
        // StatisticsDuration d("https://cmake.org/cmake/help/latest/index.html");
        GetUrlResult("cmake.org", "/cmake/help/latest/index.html");
    });

    for (auto& thread: threads) {
        thread.join();
    }

    return 0;
}