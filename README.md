# 🛠️ Fix-It-Now Service Manager

![C++](https://img.shields.io/badge/Language-C++17-blue.svg?style=for-the-badge&logo=c%2B%2B)
![Status](https://img.shields.io/badge/Status-Completed-success.svg?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg?style=for-the-badge)

> **The ultimate object-oriented solution for modern appliance repair service centers.**

---

## 🚀 Overview

**Fix-It-Now** is a robust console-based application designed to streamline the daily operations of an electronics repair shop. Built with scalability and clean architecture in mind, it manages the entire lifecycle of a service interaction—from the moment a customer walks in with a broken appliance to the final repair by a technician.

It serves as a comprehensive demonstration of **Advanced C++ concepts**, **OOP principles**, and **Design Patterns**.

## ✨ Key Features

* **👥 Advanced Personnel Management**: 
    * Hierarchy-based system: `Supervizor`, `Receptioner`, `Tehnician`.
    * Automated role validation and CNP (Personal Numeric Code) verification.
* **📺 Diverse Appliance Support**: 
    * Polymorphic handling of various devices including **TVs**, **Fridges**, and **Washing Machines**.
* **📝 Ticket System**: 
    * Full lifecycle management of Repair Requests (`CerereReparatie`).
* **⚙️ Business Logic Managers**: 
    * Dedicated managers for Employees, Appliances, and Services ensures decoupled and maintainable code.

## 🏗️ Architecture & Design Patterns

This project is built strictly following **Object-Oriented Programming (OOP)** principles to ensure modularity.

| Pattern / Concept | Implementation |
| :--- | :--- |
| **Factory Pattern** | Used in `AngajatFactory`, `ElectrocasnicFactory`, and `CerereFactory` to abstract object creation logic. |
| **Polymorphism** | Extensive use of virtual methods to handle different types of `Electrocasnic` and `Angajat` uniformly. |
| **Encapsulation** | Strict data hiding with comprehensive Getters/Setters and validation logic (e.g., `CNPValidator`). |
| **MVC-like Structure** | Separates Data (`Models`), Logic (`Managers`), and Interaction (`Meniu`). |

## 📂 Project Structure

```text
Fix-It-Now/
├── 🏭 Factories/         # Logic for creating complex objects
│   ├── AngajatFactory.cpp
│   ├── CerereFactory.cpp
│   └── ElectrocasnicFactory.cpp
├── 🧠 Managers/          # Business logic controllers
│   ├── AngajatiManager.cpp
│   ├── ServiceManager.cpp
│   └── ElectrocasniceManager.cpp
├── 👤 Models/            # Core data entities
│   ├── Angajat (Tehnician, Receptioner, etc.)
│   ├── Electrocasnic (TV, Frigider, etc.)
│   └── CerereReparatie
├── 🛠 Utilities/         # Helpers
│   ├── CNPValidator.cpp
│   └── Data.cpp
└── 🖥️ UI/
    └── Meniu.cpp         # Interactive CLI Interface
