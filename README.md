# 💾 **ztdriver** (Kernel)

![Visitors](https://api.visitorbadge.io/api/visitors?path=https%3A%2F%2Fgithub.com%2Fzabbix-byte%2Fztdriver%2F&countColor=%23263759)
[![License](https://img.shields.io/badge/License-MIT-blue.svg?style=for-the-badge&logo=windows&logoColor=white)](https://opensource.org/licenses/MIT)
[![Python Version](https://img.shields.io/badge/C++20-blue?style=for-the-badge&logo=windows&logoColor=white)](https://www.python.org/downloads/)
[![Python Version](https://img.shields.io/badge/x64-blue?style=for-the-badge&logo=windows&logoColor=white)](https://www.python.org/downloads/)
[![Github-sponsors](https://img.shields.io/badge/sponsor-30363D?style=for-the-badge&logo=GitHub-Sponsors&logoColor=white)](https://github.com/sponsors/zabbix-byte)

 💾 Kernel driver with associated user mode functionality   

### **📚 Documentation**
#### **Kernel Driver 💾**
Docs: https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk
##### **Requirements**
- https://github.com/TheCruZ/kdmapper

#### **Client usage 💻**
To use it, copy this folder in you project [lib](https://github.com/zabbix-byte/ztdriver/tree/master/ztdriver_client/ztdriver_client) .

##### **Attach to process and Detach from process**
```c++
#include "zdriver_client.hpp"

int main()
{
    if (!zdriver_client::attach_to_process(L"ac_client.exe"))
    {
        zdriver_client::detach_from_process();
        return 1;
    }
    zdriver_client::detach_from_process()
    return 0;
}
```

##### **Get module**

```c++
#include "zdriver_client.hpp"

int main()
{
	// ... first you need to Attach to process
    const uintptr_t client_module_base = zdriver_client::get_module_base(L"ac_client.exe");
    if (client_module_base == 0)
    {
        zdriver_client::detach_from_process()
        return 1;
    }
    zdriver_client::detach_from_process()
    return 0;
}
```


##### **Read Memory**

```c++
#include "zdriver_client.hpp"

int main()
{
	// ... first you need to Attach to process
    const uint32_t value = zdriver_client::read<uint32_t>(addr);
    zdriver_client::detach_from_process()
    return 0;
}
```
##### **Write Memory**

```c++
#include "zdriver_client.hpp"

int main()
{
	// ... first you need to Attach to process
    uint32_t value = 100;
    zdriver_client::write<uint32_t>(addr, value);
    zdriver_client::detach_from_process()
    return 0;
}
```

##### **Find memory address**

```c++
#include "zdriver_client.hpp"

int main()
{
	// ... first you need to Attach to process
    uint32_t addrs = zdriver_client::find_maaddy<uint32_t>(your_static, {0x0, 0x300, ...});
    zdriver_client::detach_from_process()
    return 0;
}
```

## Contributing 🤝
We welcome contributions from the community. If you'd like to contribute to mytool, please follow these guidelines:

- Fork the repository.
- Make your changes.
- Submit a pull request.

## 💌 Contact & [Sponsor](https://github.com/sponsors/zabbix-byte)

If you have any questions, suggestions, or feedback, please don't hesitate to reach out to us at [zabbix@ztrunk.space](mailto:zabbix@ztrunk.space).

We hope PyPulse accelerates your desktop application development and simplifies the integration of web content into your Python projects. Happy coding! 😎🚀
