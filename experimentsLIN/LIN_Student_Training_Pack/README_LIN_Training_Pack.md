# LIN Student Training Pack

## Files
- `LIN_Framework_Documentation.md`: prior framework with student integration notes added.
- `LIN_Signal_Encoding_Exercises.md`: signal encoding, decoding, multi-byte values, scaling, ADC callback.
- `LIN_Scheduling_and_Real_Life_Features.md`: periodic transmission and round-robin schedule.
- `LIN_Frame_Subscription_and_Identifiers.md`: frame IDs, subscribers, optional second node.
- `LIN_Fault_Injection_and_Troubleshooting.md`: safe troubleshooting exercises.
- `LIN_Alive_Counter_and_Frame_Supervision.md`: rolling counter and supervision.
- `REFERENCE_LIN.h`, `REFERENCE_LIN.c`, `REFERENCE_main.c`: copies of the supplied source files, unchanged.

## Shared assumptions
Examples use the API in the supplied driver. Keep CubeMX's generated setup and add snippets in the named `USER CODE` sections; never paste a second `main()`. Verify the baseline frame before attempting fault injection. Diagnostic IDs `0x3C`/`0x3D` are out of scope because the current driver always uses enhanced checksum. The current master driver does not implement a passive slave receiver; board-to-board reception needs separate slave firmware.
