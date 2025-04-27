# 🛡️ SystemLink: Halo-Inspired Multiplayer FPS (UE5)

**SystemLink** is a high-fidelity, Halo-style multiplayer first-person shooter built in **Unreal Engine 5**. It features fast-paced, precision-based combat, a modular weapon system, and a responsive networked gameplay architecture centered around the classic **Slayer** game mode.

---

## 🎮 Core Features

| Feature                  | Description                                                                 |
|--------------------------|-----------------------------------------------------------------------------|
| ✅ **Slayer Game Mode**     | Standard free-for-all deathmatch. First to score limit wins.              |
| 🔫 **Modular Weapon System** | Customizable per-weapon behavior using Blueprints and C++.                 |
| 💥 **Client-Side Prediction** | Responsive shooting with server-side validation for authoritative hits.   |
| 📡 **Multicast Effects**      | Muzzle flashes, hit decals, and animations synced across clients.        |
| 🎯 **Spread and Accuracy**    | Bullet cone spread and projectile counts vary per weapon.                |
| 🚀 **Optimized Networking**   | Uses batched validation with fallback timer to reduce bandwidth usage.   |

---

## 🔧 Weapon System Overview

### Local Fire

Each weapon traces locally using client-side prediction. Hit results are stored and sent to the server in bursts or on a fallback timer.

### Server Validation

The server replays the trace and applies damage only if a valid blocking hit is detected. Damage is resolved via `ApplyPointDamage`.

### Fire FX

Multicast RPCs are used to trigger fire animations and visual effects for both the weapon and character.

---

## 📦 Weapon Configuration

All weapons are defined by an exposed struct in the weapon component:

| Field                | Description                              |
|----------------------|------------------------------------------|
| `NumProjectiles`     | Number of projectiles fired per shot     |
| `ConeAngle`          | Spread angle in degrees                  |
| `MaxUnvalidatedShots`| Max client shots before flushing to server |
| `ShotValidationInterval` | Fallback flush timer for rapid fire |

---

### Example Configs

| Weapon            | NumProjectiles | ConeAngle | FireRate | ValidationInterval |
|-------------------|----------------|-----------|----------|--------------------|
| Magnum            | 1              | 0.0       | 240 RPM  | 0.25s              |
| Assault Rifle     | 1              | 3.5       | 600 RPM  | 0.15s              |
| Shotgun           | 8              | 10.0      | 60 RPM   | 1.0s               |
| Sniper Rifle      | 1              | 0.0       | 50 RPM   | 1.25s              |
| Needler           | 1              | 4.0       | 900 RPM  | 0.1s               |

---

## 🧠 Architecture Highlights

- **`USystemLinkWeaponComponent`**
    - Core C++ class managing firing, prediction, validation, and FX.
    - Blueprint-friendly via `BlueprintImplementableEvent` and `BlueprintNativeEvent`.
    - Cleanly separated into `LocalFire`, `SendShotValidation`, `ServerValidateShots`, and `MulticastPlayFireFX`.

- **Blueprint Extendability**
    - Hit events (predicted and confirmed) are Blueprint-exposed.
    - Animations, decals, sounds can be implemented with full artistic control.

- **Input System**
    - Uses **Enhanced Input** for clean, event-driven trigger pulls.

---

