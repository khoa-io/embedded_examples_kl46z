FRDM-KL46Z Memory Layout (With Bootloader)
=========

| Address Range               | Content                       |        Length |
|-----------------------------|-------------------------------|--------------:|
| `0x0000_0000 - 0x0000_0100` | `m_interrupts` (Bootloader)   | `0x0000_0100` |
| `0x0000_0400 - 0x0000_0410` | `m_flash_config` (Bootloader) | `0x0000_0010` |
| `0x0000_0410 - 0x0004_0000` | `m_text` (Bootloader)         | `0x0003_FBF0` |
| `0x0000_A000 - 0x0000_A100` | `m_interrupts` (App)          | `0x0000_0100` |
| `0x0000_A400 - 0x0000_A410` | `m_flash_config` (App)        | `0x0000_0010` |
| `0x0000_A410 - 0x0004_A000` | `m_text` (App)                | `0x0003_FBF0` |
| ...                         | ...                           |           ... |
| `0x1FFF_E000 - 0x2000_6000` | `m_data`                      | `0x0000_8000` |
|                             |                               |               |
