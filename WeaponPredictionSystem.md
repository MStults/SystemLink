# SystemLink Weapon Prediction System

This document outlines the design and implementation of the client-side prediction and server-side validation system used for shooting mechanics in **SystemLink**.

---

## ✨ Goals

- Predict hit results on the **client** for immediate responsiveness.
- **Accumulate and send** shot data to the **server** in batches.
- Server performs **authoritative validation**, applies damage, and triggers FX using multicast.
- Allow per-weapon tuning for fire rate, cone angle, shot threshold, etc.
- Ensure client-side predicted hits are reconciled with server-confirmed hits.
- Provide visual and debug tools to audit prediction accuracy and diagnose desync.

---

## 📦 Core Components

| Component                    | Description                                                                 |
|-----------------------------|-----------------------------------------------------------------------------|
| `LocalFire()`               | Client-side prediction logic. Traces locally and stores `ShotHistory`.      |
| `SendShotValidation()`      | Sends accumulated shots to the server. Triggered by threshold or timer.     |
| `ServerValidateShots()`     | Runs traces on server, applies damage, and triggers FX.                     |
| `MulticastPlayFireFX()`     | Multicast for weapon and character animations, muzzle flash, etc.           |
| `OnProjectileHitPredicted()`| Blueprint event for predicted hits (client-side).                          |
| `OnProjectileHitConfirmed()`| Blueprint event for confirmed hits (server-side).                          |
| `GenerateBulletSpread()`    | Utility to calculate projectile spread direction(s).                        |
| `FShotInfo`                 | Struct storing shot metadata including position, direction, and timestamp.  |
| `FConfirmedProjectileHit`   | Struct returned by server validation. Used to confirm and apply damage.     |
| `MatchPredictedShotToConfirmed()` | Logic to reconcile predicted vs confirmed hits using `Timestamp`.        |

---

## 🔧 Weapon Spread & Pellet Configuration

| Weapon Type      | NumProjectiles | ConeAngle (°) | Notes                                         |
|------------------|----------------|----------------|-----------------------------------------------|
| **Magnum**        | 1              | 0.0            | Perfectly accurate single shot                |
| **Assault Rifle** | 1              | 3.5            | Slight spread per bullet (automatic fire)     |
| **Shotgun**       | 8              | 10.0           | Wide spread to simulate buckshot              |
| **Sniper Rifle**  | 1              | 0.0            | Pinpoint accuracy                             |
| **Plasma Rifle**  | 1              | 2.0            | Slight variance, energy weapon                |
| **SMG (Dual)**    | 1              | 5.0            | High fire rate, moderate inaccuracy           |
| **Needler**       | 1              | 4.0            | Homing projectiles with slight spread         |
| **Rocket Launcher**| 1             | 0.0            | Straight-line explosive projectile            |

---

## 🧮 Suggested `ShotValidationInterval` Values

| Weapon Type      | Fire Rate (RPM) | Time Between Shots (s) | Suggested Interval | Notes                                         |
|------------------|-----------------|-------------------------|--------------------|-----------------------------------------------|
| **Magnum**        | 240             | 0.25                    | `0.25f`            | One shot per trigger                          |
| **Battle Rifle**  | 450             | 0.13                    | `0.15f`            | Short burst fire                              |
| **Assault Rifle** | 600             | 0.10                    | `0.15f`            | Auto fire, moderate batching                  |
| **Shotgun**       | 60              | 1.00                    | `1.0f`             | Powerful single fire                          |
| **Sniper Rifle**  | 50              | 1.20                    | `1.25f`            | Low frequency, full validation per shot       |
| **Needler**       | 900             | 0.066                   | `0.1f`             | Very rapid fire, frequent validation needed   |
| **Plasma Pistol** | Varies          | Varies (charge-based)   | `0.2f`             | Depends on mode (charge vs burst)             |

---

### 🔵 `HitValidationSweepRadius` Defaults per Weapon

| Weapon         | HitValidationSweepRadius | Notes                                              |
|----------------|--------------------------|----------------------------------------------------|
| Sniper Rifle   | `0.0f`                   | No leeway – must be precise                       |
| Magnum         | `5.0f`                   | Small buffer for minor input variance             |
| Assault Rifle  | `8.0f`                   | Forgiveness for high fire rate                    |
| Shotgun        | `20.0f`                  | Large radius to catch scattered pellets           |
| Needler        | `10.0f`                  | Projectiles may curve slightly, helps validation  |

---

## 🎛️ Tunable Variables

| Variable                   | Description                                                                 |
|----------------------------|-----------------------------------------------------------------------------|
| `NumProjectiles`           | Projectiles per shot. E.g. 1 for rifles, 8+ for shotgun.                    |
| `ConeAngle`                | Spread angle in degrees. Affects projectile dispersion.                     |
| `MaxUnvalidatedShots`      | Threshold for how many local shots to store before flushing to server.      |
| `ShotValidationInterval`   | Fallback timer to ensure timely server sync on high fire-rate weapons.      |
| `HitValidationSweepRadius` | Radius used in server-side validation sweep to account for near-miss leniency. |
| `MaxShotReconciliationTime`| Time in seconds to keep predicted shots in memory for comparison.          |

---

## 🔁 Shot Flow Overview

```text
[Client Pulls Trigger]
        ↓
    LocalFire()
        ↓
[Client Traces and Predicts]
        ↓
ShotHistory.Add(FShotInfo)
        ↓
[Threshold Reached or Timer Fires]
        ↓
 SendShotValidation()
        ↓
[Server Validates and Traces]
        ↓
ServerValidateShots()
        ↓
ApplyPointDamage()
→ OnProjectileHitConfirmed()
→ MulticastPlayFireFX()
        ↓
MatchPredictedShotToConfirmed()
```

