//
// Created by 72951 on 2024/9/29.
//

#ifndef QTUPGRADETOOLS_PAYLOAD_H
#define QTUPGRADETOOLS_PAYLOAD_H

#include <concepts>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace whisper
{
    // payload 类用于在内存中存储字节序列，提供读写操作的接口
    struct payload
    {
        // 默认构造函数
        payload() = default;

        // 从 std::string 构造 payload 对象
        // 参数: str - 输入的字符串
        explicit payload(const std::string& str)
        {
            // 根据字符串大小调整数据缓冲区
            data_.resize(str.size(), {});
            // 将字符串内容复制到数据缓冲区
            std::memcpy(data_.data(), str.data(), str.size());
        }

        // 从原始缓冲区和大小构造 payload 对象
        // 参数: buffer - 指向原始数据的指针, size - 数据大小
        payload(const void *buffer, const size_t size)
        {
            // 调整数据缓冲区大小
            data_.resize(size, {});
            // 将原始缓冲区内容复制到数据缓冲区
            std::memcpy(data_.data(), buffer, size);
        }


        // 从固定大小的 uint8_t 数组构造 payload 对象
        // 参数: buffer - 输入的数组
        template<size_t N>
        explicit payload(const std::array<uint8_t, N>& buffer)
        {
            // 根据数组大小调整数据缓冲区
            data_.resize(buffer.size(), {});
            // 将数组内容复制到数据缓冲区
            std::memcpy(data_.data(), buffer.data(), buffer.size());
        }

        // 从初始化列表构造 payload 对象
        // 参数: list - 字节初始化列表
        payload(const std::initializer_list<uint8_t> list)
        {
            // 根据列表大小调整数据缓冲区
            data_.resize(list.size(), {});
            // 将列表内容复制到数据缓冲区
            std::memcpy(data_.data(), std::data(list), list.size());
        }

        // 写入任意算术类型或枚举到 payload
        // 参数: value - 要写入的值
        template<typename T>
        requires std::is_arithmetic_v<T> || std::is_enum_v<T>
        payload& write(T value)
        {
            // 当前写入索引
            const size_t widx = data_.size();

            // 扩展数据缓冲区以容纳新数据
            data_.resize(widx + sizeof(T), {});

            // 将值写入数据缓冲区
            std::memcpy(data_.data() + widx, &value, sizeof(T));

            return *this; // 返回自身以支持链式调用
        }

        // 从原始缓冲区写入数据
        // 参数: buffer - 指向原始数据的指针, size - 数据大小
        payload& write(const void *buffer, const size_t size)
        {
            // 当前写入索引
            const size_t widx = data_.size();

            // 扩展数据缓冲区以容纳新数据
            data_.resize(widx + size, {});

            // 将缓冲区数据写入
            std::memcpy(data_.data() + widx, buffer, size);

            return *this; // 返回自身以支持链式调用
        }

        // 读取当前索引后的数据
        size_t ridx{}; // 当前读取索引

        // 读取任意算术类型或枚举类型
        template<typename T>
        requires std::is_arithmetic_v<T> || std::is_enum_v<T>
        T read()
        {
            // 检查是否超出范围
            if ((ridx + sizeof(T)) > data_.size()) return T{};

            // 从数据缓冲区读取数据
            T value = *reinterpret_cast<T *>(data_.data() + ridx);

            ridx += sizeof(T); // 更新读取索引

            return value; // 返回读取的值
        }

        // 忽略字节序，读取类类型数据
        template<typename T>
        requires std::is_class_v<T>
        T read()
        {
            // 检查是否超出范围
            if ((ridx + sizeof(T)) > data_.size()) return T{};

            // 从数据缓冲区读取数据
            T value = *reinterpret_cast<T *>(data_.data() + ridx);

            ridx += sizeof(T); // 更新读取索引

            return value; // 返回读取的值
        }

        // 清空数据缓冲区及重置读取索引
        void clear()
        {
            data_.clear(); // 清空数据缓冲区
            ridx = 0; // 重置读取索引
        }

        // 获取当前数据缓冲区的大小
        [[nodiscard]] auto size() const noexcept { return data_.size(); }

        // 获取数据缓冲区的原始指针
        [[nodiscard]] const uint8_t *data() const noexcept { return data_.data(); }

    private:
        std::vector<uint8_t> data_{}; // 数据缓冲区，以字节存储
    };
} // namespace whisper

#endif //QTUPGRADETOOLS_PAYLOAD_H
