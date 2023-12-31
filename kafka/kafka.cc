#include <iostream>
#include <fstream>
#include <string>
#include <librdkafka/rdkafkacpp.h>

// Kafka配置
const std::string kafkaBroker = "10.1.0.106:9092";
const std::string kafkaTopic = "log";

int main() {
    // Kafka配置
    std::string errstr;

    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("metadata.broker.list", kafkaBroker, errstr);

    // 创建生产者
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::cerr << "Failed to create producer: " << errstr << std::endl;
        return -1;
    }

    // 打开文件
    std::ifstream file_1("../results/recv_log");
    std::ifstream file_2("../results/send_log");
    if (!file_1.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return -1;
    }

    // 逐行读取文件并发送到Kafka
    std::string line;
    while (std::getline(file_1, line)) {
        RdKafka::ErrorCode resp = producer->produce(
            kafkaTopic, 
            RdKafka::Topic::PARTITION_UA, 
            RdKafka::Producer::RK_MSG_COPY, 
            const_cast<char *>(line.c_str()), 
            line.size(), 
            NULL, 
            0, 
            0, 
            NULL
        );

        if (resp != RdKafka::ERR_NO_ERROR) {
            std::cerr << "Failed to produce message: " << RdKafka::err2str(resp) << std::endl;
        }
    }
    while (std::getline(file_2, line)) {
        RdKafka::ErrorCode resp = producer->produce(
            kafkaTopic, 
            RdKafka::Topic::PARTITION_UA, 
            RdKafka::Producer::RK_MSG_COPY, 
            const_cast<char *>(line.c_str()), 
            line.size(), 
            NULL, 
            0, 
            0, 
            NULL
        );

        if (resp != RdKafka::ERR_NO_ERROR) {
            std::cerr << "Failed to produce message: " << RdKafka::err2str(resp) << std::endl;
        }
    }

    // 关闭文件和Kafka生产者
    file_1.close();
    file_2.close();
    delete producer;

    return 0;
}
