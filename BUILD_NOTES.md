# UnNetHack Android Build Notes
## Date: December 27, 2025

## Build Status: ✅ SUCCESSFUL

### APK Location
**Signed Release APK:** `/home/thomas/Downloads/UnNetHack-Android/sys/android/build/outputs/apk/release/UnNetHack-Android-release.apk`
- Size: 9.0 MB
- Architecture: arm64-v8a (64-bit)
- Signed with: Same keystore as NetHack (`/home/thomas/.keystore/nethack-release.jks`)

### Critical 64-bit Compatibility Fixes Applied

#### 1. Updated `anything` Union (include/wintype.h)
Added `a_long` field to support 64-bit pointers:
```c
typedef union any {
    genericptr_t a_void;
    struct obj *a_obj;
    long a_long;        // ← ADDED for 64-bit
    int  a_int;
    char a_char;
    schar a_schar;
} anything;
```

#### 2. Fixed JNI Menu Handling (sys/android/winandroid.c)

**Line 858 - Menu item addition:**
```c
// Changed from: ident->a_int
JNICallV(jAddMenu, wid, tile, ident->a_long, ...);
```

**Lines 934-941 - Menu selection retrieval:**
```c
// Changed from: GetIntArrayElements
q = p = (*jEnv)->GetLongArrayElements(jEnv, a, 0);
*selected = (MENU_ITEM_P*)malloc(sizeof(MENU_ITEM_P) * n);
for(i = 0; i < n; i++)
{
    // Changed from: a_int
    (*selected)[i].item.a_long = *p++;
    (*selected)[i].count = *p++;
}
// Changed from: ReleaseIntArrayElements
(*jEnv)->ReleaseLongArrayElements(jEnv, a, q, 0);
```

#### 3. Fixed Build Tools (util/Makefile)
Removed `-m32` flag from CFLAGS to allow native 64-bit compilation:
```makefile
# Changed from: -m32 -DHAVE_STDINT_H=1
CFLAGS = -I../include -I$(srcdir)/../include -O2 -Wno-format-security -DHAVE_STDINT_H=1 -DANDROID
```

### Build Configuration Updates

#### Android Gradle Plugin Modernization
**File:** `sys/android/build.gradle`
- Updated to AGP 9.0.0-rc01
- Added signing configuration with keystore
- Set Java 17 compatibility
- Namespace: `com.tbd.UnNetHack`
- Version: 5.3.2-1 (versionCode 5321)

#### Gradle Wrapper
**File:** `sys/android/gradle/wrapper/gradle-wrapper.properties`
- Updated from Gradle 5.2.1 to 9.2.1

#### Android Manifest
**File:** `sys/android/AndroidManifest.xml`
- Removed deprecated `<uses-sdk>` tag
- Removed `android:versionCode` and `android:versionName` (moved to build.gradle)

#### Settings
**File:** `sys/android/settings.gradle`
- Added modern plugin management
- Configured dependency resolution
- Added ForkFront library integration via includeBuild

**File:** `sys/android/local.properties`
- Set SDK location: `/home/thomas/Android/Sdk`

**File:** `sys/android/keystore.properties`
- Configured signing with NetHack keystore

### Compilation Flags
**File:** `sys/android/Makefile.src`
- CFLAGS: `-Wall -Wno-error=incompatible-pointer-types -I../include -I$(srcdir)/../include -g -O2 -Wno-format -fsigned-char -DANDROID -fPIC`
- Architecture: arm64-v8a
- NDK: `/home/thomas/Downloads/ndk/29.0.14206865`
- Compiler: `aarch64-linux-android35-clang`

### Build Commands

#### Native Library Build
```bash
cd /home/thomas/Downloads/UnNetHack-Android
make clean
make install
```

#### APK Build
```bash
export JAVA_HOME=/usr/lib/jvm/java-21-openjdk
cd /home/thomas/Downloads/UnNetHack-Android/sys/android
./gradlew clean assembleRelease
```

### Key Differences from NetHack Build

1. **Menu item identifiers:** UnNetHack now uses `a_long` matching NetHack's 64-bit compatible approach
2. **JNI types:** All menu-related JNI calls use `jlong` instead of `jint` for proper 64-bit support
3. **Build tools:** Removed 32-bit compilation restriction

### Architecture Details

**Native Libraries:**
- Location: `sys/android/libs/arm64-v8a/`
- Library: `libunnethack.so` (6.4 MB)
- Old 32-bit library removed: `libs/armeabi/` deleted

### Testing Notes

The previous crash was caused by:
- Menu item identifiers being truncated from 64-bit pointers to 32-bit integers
- JNI type mismatch between Java's `long[]` (64-bit) and C's `int` (32-bit) on ARM64
- This caused menu selections to return corrupted memory addresses

The fixes ensure proper type compatibility across the 64-bit JNI boundary.

### Dependencies

- **ForkFront Library:** Referenced from NetHack build at `../../../NetHack-Android/sys/forkfront`
- Version: 1.1
- Maven repository: JitPack.io

### Environment

- OS: Linux (Fedora-based)
- Java: OpenJDK 21 & 25
- NDK: 29.0.14206865
- Android SDK: `/home/thomas/Android/Sdk`
- Compile SDK: 34
- Min SDK: 21
- Target SDK: 34

### References

- NetHack Android: `/home/thomas/Downloads/NetHack-Android`
- NetHack APK (for comparison): `/home/thomas/Downloads/NetHack-Android/sys/android/app/build/outputs/apk/release/app-release.apk`

### Next Steps (if needed)

1. Test the APK on a physical device
2. If issues persist, check logcat for native crashes
3. Verify menu interactions work correctly
4. Consider building debug APK with: `./gradlew assembleDebug`

---
**Build completed:** December 27, 2025
**Status:** Ready for installation and testing
