/* Faithfallguys 
 *
 * Simple meme hack for Fall Guys
 * Please do not use online to ruin games
 *
*/

#include "imports.h"
#include "tools.h"
#include "offsets.h"

int main() {
    int pid = 0;

    printf("Welcome to Faithfallguys!\n");
    printf("Written by Lunarixus as a proof of concept\n");
    printf("Please do not use online, I am not responsible if you get banned.\n\n\n");

    /* Get process ID */
    printf("[INFO]: Getting process ID of Fall Guys game...\n");
    pid = GetWindowPID("FallGuys_client");
    if (pid != 0) {
        printf("[INFO]: Process ID found: %d\n", pid);
    }
    else {
        abort();
    }

    /* Get handle to Fall Guys process */
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    /* Get base address of GameAssembly DLL */
    printf("[INFO]: Getting base address of GameAssembly.dll...\n");
    uintptr_t GameAssemblyBaseAddr = GetModuleBaseAddress(pid, L"GameAssembly.dll");
    if (GameAssemblyBaseAddr != NULL) {
        printf("[INFO]: Got base address of GameAssembly.dll: 0x%p\n", GameAssemblyBaseAddr);
    }

    /* To test everything is fine, get normalMaxSpeed */
    uintptr_t maxspeedaddr = AddPointers(hProcess, GameAssemblyBaseAddr + 0x02886C10, normalMaxSpeed_offsets);
    float normalMaxSpeed_value;
    printf("[INFO]: normalMaxSpeed address is: 0x%p\n", maxspeedaddr);
    ReadProcessMemory(hProcess, (LPCVOID)(maxspeedaddr), &normalMaxSpeed_value, sizeof(float), NULL);
    if (normalMaxSpeed_value != 9.500000) {
        WriteProcessMemory(hProcess, (LPVOID)maxspeedaddr, "9.500000", sizeof("9.500000"), NULL);
    }
    if (normalMaxSpeed_value == 9.500000) {
        printf("[INFO]: Got normalMaxSpeed value! %f\n\n", normalMaxSpeed_value);
    }

#if 0
    /* Read out entire PlayerControllerData struct */
    for (int i = 0; i < 100; i++) {
        float value = 0;
        ReadProcessMemory(hProcess, (LPCVOID)(maxspeedaddr + (i * 0x4)), &value, sizeof(float), NULL);
        printf("[INFO]: Value at 0x%p is %f\n", value, value);
    }
#endif

    /* Get value for GravityScale */
    uintptr_t gravityscaleaddr = maxspeedaddr + 0xA0;
    float gravityscale_value;
    printf("[INFO]: gravityScale address is: 0x%p\n", gravityscaleaddr);
    ReadProcessMemory(hProcess, (LPCVOID)(gravityscaleaddr), &gravityscale_value, sizeof(float), NULL);
    printf("[INFO]: Got gravityScale value! %f\n\n", gravityscale_value);

    /* Get value for jumpforce */
    uintptr_t jumpforceaddr = gravityscaleaddr + 0x10;
    int jumpforce_value[4];
    printf("[INFO]: jumpforce address is: 0x%p\n", jumpforceaddr);
    ReadProcessMemory(hProcess, (LPCVOID)(jumpforceaddr), &jumpforce_value, sizeof(jumpforce_value), NULL);
    printf("[INFO]: Got jumpforce value! %d\n\n", jumpforce_value);

    /* Disable grab cooldown */
    uintptr_t grabcooldownaddr = jumpforceaddr + 0x84;
    float grabcooldown_value;
    printf("[INFO]: grabCooldown address is: 0x%p\n", grabcooldownaddr);
    ReadProcessMemory(hProcess, (LPCVOID)(grabcooldownaddr), &grabcooldown_value, sizeof(jumpforce_value), NULL);
    printf("[INFO]: Got grabCooldown value! %f\n\n", grabcooldown_value);

    /* Print which hacks are currently available */
    printf("[NUMPAD0] Speedhack\n");
    printf("[NUMPAD1] GravityHack\n");

    /* Set up variables for the hacks */
    bool speedhack_on = false;
    float newMaxSpeed_value = 25.500000;
    float oldMaxSpeed_value = 9.500000;

    bool gravityhack_on = false;
    float newGravityScale_value = -0.500000;
    float oldGravityScale_value = 1.500000;

    /* Start loop to look for keystates */
    while (1) {
        /* Constantly check if key is pressed */
        if (GetAsyncKeyState(VK_NUMPAD0) & 1)
        {
            speedhack_on = !speedhack_on;
            if (speedhack_on == true) {
                printf("[INFO]: Speed hack toggled on\n");
            }
            else if (speedhack_on == false) {
                printf("[INFO]: Speed hack toggled off\n");
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            gravityhack_on = !gravityhack_on;
            if (gravityhack_on == true) {
                printf("[INFO]: Gravity hack toggled on\n");
            }
            else if (gravityhack_on == false) {
                printf("[INFO]: Gravity hack toggled off\n");
            }
        }

        /* Do Hax */
        if (speedhack_on == true) {
            WriteProcessMemory(hProcess, (LPVOID)maxspeedaddr, &newMaxSpeed_value, sizeof(newMaxSpeed_value), NULL);
        }
        else {
            WriteProcessMemory(hProcess, (LPVOID)maxspeedaddr, &oldMaxSpeed_value, sizeof(newMaxSpeed_value), NULL);
        }

        if (gravityhack_on == true) {
            WriteProcessMemory(hProcess, (LPVOID)gravityscaleaddr, &newGravityScale_value, sizeof(newGravityScale_value), NULL);
        }
        else {
            WriteProcessMemory(hProcess, (LPVOID)gravityscaleaddr, &oldGravityScale_value, sizeof(newGravityScale_value), NULL);
        }

        Sleep(1);
    }

}