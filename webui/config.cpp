#include "config.hpp"
#include "json.hpp"

#include "umod/runtime/helper/input.hpp"
#include "user/config.hpp"

#include <array>
#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace
{
    std::string escaped(const std::string &s)
    {
        std::string out;
        for (char c : s)
        {
            switch (c)
            {
            case '\"':
                out += "\\\"";
                break;
            case '\\':
                out += "\\\\";
                break;
            case '\n':
                out += "\\n";
                break;
            case '\r':
                out += "\\r";
                break;
            case '\t':
                out += "\\t";
                break;
            default:
                out += c;
            }
        }
        return out;
    }

    // string
    std::string jsonify(const std::string &v) { return "\"" + escaped(v) + "\""; }
    // bool
    std::string jsonify(bool v) { return v ? "true" : "false"; }
    // number
    template <typename T>
        requires std::is_arithmetic_v<T> && (!std::is_same_v<T, bool>)
    std::string jsonify(T v)
    {
        return std::to_string(v);
    }
    // duration
    std::string jsonify(const std::chrono::duration<float> &v)
    {
        // expose duration as seconds
        return std::to_string(v.count());
    }
    // array
    template <typename T, size_t N>
    std::string jsonify(const std::array<T, N> &v)
    {
        std::string json = "[";
        for (size_t i = 0; i < v.size(); ++i)
        {
            json += jsonify(v[i]);
            if (i + 1 < v.size()) json += ",";
        }
        json += "]";
        return json;
    }
    template <typename T>
    std::string jsonify(const std::vector<T> &v)
    {
        std::string json = "[";
        for (size_t i = 0; i < v.size(); ++i)
        {
            json += jsonify(v[i]);
            if (i + 1 < v.size()) json += ",";
        }
        json += "]";
        return json;
    }
    //key
    std::string jsonify(const user_config::freecam::keybind::Key &v)
    {

#define QUOTE(x) "\"" #x "\""
#define RET_KEY_STR(key)                                                                                               \
    case umod::unity_runtime::helper::InputUtils::Key::key:                                                            \
        return QUOTE(key);

        switch (v)
        {
            RET_KEY_STR(A)
            RET_KEY_STR(B)
            RET_KEY_STR(C)
            RET_KEY_STR(D)
            RET_KEY_STR(E)
            RET_KEY_STR(F)
            RET_KEY_STR(G)
            RET_KEY_STR(H)
            RET_KEY_STR(I)
            RET_KEY_STR(J)
            RET_KEY_STR(K)
            RET_KEY_STR(L)
            RET_KEY_STR(M)
            RET_KEY_STR(N)
            RET_KEY_STR(O)
            RET_KEY_STR(P)
            RET_KEY_STR(Q)
            RET_KEY_STR(R)
            RET_KEY_STR(S)
            RET_KEY_STR(T)
            RET_KEY_STR(U)
            RET_KEY_STR(V)
            RET_KEY_STR(W)
            RET_KEY_STR(X)
            RET_KEY_STR(Y)
            RET_KEY_STR(Z)
            RET_KEY_STR(UpArrow)
            RET_KEY_STR(DownArrow)
            RET_KEY_STR(RightArrow)
            RET_KEY_STR(LeftArrow)
            RET_KEY_STR(Shift_L)
            RET_KEY_STR(Shift_R)
            RET_KEY_STR(Ctrl_L)
            RET_KEY_STR(Ctrl_R)
            RET_KEY_STR(Backspace)
            RET_KEY_STR(Tab)
            RET_KEY_STR(Enter)
            RET_KEY_STR(Space)
            RET_KEY_STR(Minus)
            RET_KEY_STR(Equals)
            RET_KEY_STR(Esc)
        }
    }
    // Camera Mode
    std::string jsonify(const user_config::freecam::CameraMode &v)
    {
#define RET_CAMERA_MODE_STR(mode)                                                                                      \
    case user_config::freecam::CameraMode::mode:                                                                       \
        return QUOTE(mode);

        switch (v)
        {
            RET_CAMERA_MODE_STR(Orignal)
            RET_CAMERA_MODE_STR(Depth)
            RET_CAMERA_MODE_STR(MainCamera)
        }
    }

    std::optional<user_config::freecam::keybind::Key> key_from_literal(const std::string &literal)
    {
#define RET_STR_KEY(str)                                                                                               \
    if (literal == #str) return user_config::freecam::keybind::Key::str;
        RET_STR_KEY(A)
        RET_STR_KEY(B)
        RET_STR_KEY(C)
        RET_STR_KEY(D)
        RET_STR_KEY(E)
        RET_STR_KEY(F)
        RET_STR_KEY(G)
        RET_STR_KEY(H)
        RET_STR_KEY(I)
        RET_STR_KEY(J)
        RET_STR_KEY(K)
        RET_STR_KEY(L)
        RET_STR_KEY(M)
        RET_STR_KEY(N)
        RET_STR_KEY(O)
        RET_STR_KEY(P)
        RET_STR_KEY(Q)
        RET_STR_KEY(R)
        RET_STR_KEY(S)
        RET_STR_KEY(T)
        RET_STR_KEY(U)
        RET_STR_KEY(V)
        RET_STR_KEY(W)
        RET_STR_KEY(X)
        RET_STR_KEY(Y)
        RET_STR_KEY(Z)
        RET_STR_KEY(UpArrow)
        RET_STR_KEY(DownArrow)
        RET_STR_KEY(RightArrow)
        RET_STR_KEY(LeftArrow)
        RET_STR_KEY(Shift_L)
        RET_STR_KEY(Shift_R)
        RET_STR_KEY(Ctrl_L)
        RET_STR_KEY(Ctrl_R)
        RET_STR_KEY(Backspace)
        RET_STR_KEY(Tab)
        RET_STR_KEY(Enter)
        RET_STR_KEY(Space)
        RET_STR_KEY(Minus)
        RET_STR_KEY(Equals)
        RET_STR_KEY(Esc)
        return std::nullopt;
    }
    std::optional<user_config::freecam::CameraMode> camera_mode_from_literal(const std::string &literal)
    {
#define RET_STR_CAMERA_MODE(str)                                                                                       \
    if (literal == #str) return user_config::freecam::CameraMode::str;
        RET_STR_CAMERA_MODE(Orignal)
        RET_STR_CAMERA_MODE(Depth)
        RET_STR_CAMERA_MODE(MainCamera)
        return std::nullopt;
    }
}

// Camera Mode
#define M(Literal)                                                                                                     \
    if (auto it = value->find(#Literal); it != value->end())                                                           \
        if (const auto value = it->second.as<std::string>())                                                           \
            if (auto mode = camera_mode_from_literal(*value)) Literal = *mode;
// Key
#define K(Literal)                                                                                                     \
    if (auto it = value->find(#Literal); it != value->end())                                                           \
        if (const auto value = it->second.as<std::string>())                                                           \
            if (auto key = key_from_literal(*value)) Literal = *key;
// Bool
#define B(Literal)                                                                                                     \
    if (auto it = value->find(#Literal); it != value->end())                                                           \
        if (const auto value = it->second.as<bool>()) Literal = *value;

// Chrono
#define C(Literal)                                                                                                     \
    if (auto it = value->find(#Literal); it != value->end())                                                           \
        if (const auto value = it->second.as<double>()) Literal = std::chrono::duration<float>(*value);

// Number
#define N(Literal)                                                                                                     \
    if (auto it = value->find(#Literal); it != value->end())                                                           \
        if (const auto value = it->second.as<double>()) Literal = *value;

// Array String
#define AS(Literal)                                                                                                    \
    if (auto it = value->find(#Literal); it != value->end())                                                           \
        if (const auto value = it->second.as<Json::Array>())                                                           \
        {                                                                                                              \
            std::vector<std::string> vs{};                                                                             \
            for (const auto &v : *value)                                                                               \
                if (const auto &s = v.as<std::string>()) vs.push_back(*s);                                             \
            Literal = vs;                                                                                              \
        }

namespace user_config
{
    using json::Json;

    // ====== user_config ======
    std::string to_json()
    {
        std::ostringstream os;
        os << "{";
        os << "\"Loader\":" << loader::to_json() << ",";
        os << "\"Core\":" << core::to_json() << ",";
        os << "\"FreeCam\":" << freecam::to_json() << ",";
        os << "\"Speed Gear\":" << speed_gear::to_json();
        os << "}";
        return os.str();
    }
    void from_json(const Json &data)
    {
        if (const auto value = data.as<Json::Object>())
        {
            if (auto it = value->find("Loader"); it != value->end()) loader::from_json(it->second);
            if (auto it = value->find("Core"); it != value->end()) core::from_json(it->second);
            if (auto it = value->find("FreeCam"); it != value->end()) freecam::from_json(it->second);
            if (auto it = value->find("Speed Gear"); it != value->end()) speed_gear::from_json(it->second);
        }
    }

    namespace loader
    {
        // ====== user_config::loader ======
        std::string to_json()
        {
            std::ostringstream os;
            os << "{";
            os << "\"Hardened\":" << jsonify(loader::Hardened) << ",";
            os << "\"WaitBeforeInit\":" << jsonify(loader::WaitBeforeInit);
            os << "}";
            return os.str();
        }
        void from_json(const Json &data)
        {
            if (const auto value = data.as<Json::Object>())
            {
                B(Hardened)
                C(WaitBeforeInit)
            }
        }
    }

    namespace core
    {
        // ====== user_config::core ======
        std::string to_json()
        {
            std::ostringstream os;
            os << "{";
            os << "\"MockLoopDeltaTime\":" << jsonify(core::MockLoopDeltaTime) << ",";
            os << "\"EnabledModules\":" << jsonify(core::EnabledModules);
            os << "}";
            return os.str();
        }
        void from_json(const Json &data)
        {
            if (const auto value = data.as<Json::Object>())
            {
                C(MockLoopDeltaTime)
                AS(EnabledModules)
            }
        }
    }

    namespace freecam
    {
        // ====== user_config::freecam ======
        std::string to_json()
        {
            std::ostringstream os;
            os << "{";
            os << "\"Mode\":" << jsonify(Mode) << ",";
            os << "\"property\":" << property::to_json() << ",";
            os << "\"keybind\":" << keybind::to_json();
            os << "}";
            return os.str();
        }
        void from_json(const Json &data)
        {
            if (const auto value = data.as<Json::Object>())
            {
                M(Mode)
                B(DisableOrigCam)
                if (auto it = value->find("property"); it != value->end()) property::from_json(it->second);
                if (auto it = value->find("keybind"); it != value->end()) keybind::from_json(it->second);
            }
        }

        namespace property
        {
            // ====== user_config::freecam::property ======
            std::string to_json()
            {
                std::ostringstream os;
                os << "{";
                os << "\"RotationSpeed\":" << jsonify(RotationSpeed) << ",";
                os << "\"RollSpeed\":" << jsonify(RollSpeed) << ",";
                os << "\"BaseMoveSpeed\":" << jsonify(BaseMoveSpeed) << ",";
                os << "\"MaxMoveSpeed\":" << jsonify(MaxMoveSpeed);
                os << "}";
                return os.str();
            }
            void from_json(const Json &data)
            {
                if (const auto value = data.as<Json::Object>())
                {
                    N(RotationSpeed)
                    N(RollSpeed)
                    N(BaseMoveSpeed)
                    N(MaxMoveSpeed)
                }
            }
        }

        namespace keybind
        {
            // ====== user_config::freecam::keybind ======
            std::string to_json()
            {
                std::ostringstream os;
                os << "{";
                os << "\"ToggleFreeCam\":" << jsonify(keybind::ToggleFreeCam) << ",";

                os << "\"Up\":" << jsonify(keybind::Up) << ",";
                os << "\"Down\":" << jsonify(keybind::Down) << ",";
                os << "\"Forward\":" << jsonify(keybind::Forward) << ",";
                os << "\"Back\":" << jsonify(keybind::Back) << ",";
                os << "\"Left\":" << jsonify(keybind::Left) << ",";
                os << "\"Right\":" << jsonify(keybind::Right) << ",";
                os << "\"SpeedUp\":" << jsonify(keybind::SpeedUp) << ",";

                os << "\"RotateUp\":" << jsonify(keybind::RotateUp) << ",";
                os << "\"RotateDown\":" << jsonify(keybind::RotateDown) << ",";
                os << "\"RotateLeft\":" << jsonify(keybind::RotateLeft) << ",";
                os << "\"RotateRight\":" << jsonify(keybind::RotateRight) << ",";
                os << "\"RollLeft\":" << jsonify(keybind::RollLeft) << ",";
                os << "\"RollRight\":" << jsonify(keybind::RollRight) << ",";
                os << "\"ResetRoll\":" << jsonify(keybind::ResetRoll) << ",";

                os << "\"ZoomMode\":" << jsonify(keybind::ZoomMode) << ",";
                os << "\"ZoomIn\":" << jsonify(keybind::ZoomIn) << ",";
                os << "\"ZoomOut\":" << jsonify(keybind::ZoomOut) << ",";

                os << "\"PinAnchor\":" << jsonify(keybind::PinAnchor) << ",";

                os << "\"UIMode\":" << jsonify(keybind::UIMode) << ",";
                os << "\"AttachMode\":" << jsonify(keybind::AttachMode);
                os << "}";
                return os.str();
            }
            void from_json(const Json &data)
            {
                if (const auto value = data.as<Json::Object>())
                {
                    // clang-format off
                    K(ToggleFreeCam)
                    K(Down) K(Forward) K(Back) K(Left) K(Right) K(SpeedUp)
                    K(RotateUp) K(RotateDown) K(RotateLeft) K(RollLeft) K(RollRight) K(ResetRoll)
                    K(ZoomMode) K(ZoomIn) K(ZoomOut)
                    K(PinAnchor)
                    K(UIMode) K(AttachMode)
                    // clang-format on
                }
            }
        }
    }

    namespace speed_gear
    {
        // ====== user_config::speed_gear ======
        std::string to_json()
        {
            std::ostringstream os;
            os << "{";
            os << "\"keybind\":" << keybind::to_json();
            os << "}";
            return os.str();
        }
        void from_json(const Json &data)
        {
            if (const auto value = data.as<Json::Object>())
            {
                if (auto it = value->find("keybind"); it != value->end()) keybind::from_json(it->second);
            }
        }

        namespace keybind
        {
            // ====== user_config::speed_gear::keybind ======
            std::string to_json()
            {
                std::ostringstream os;
                os << "{";
                os << "\"ToggleFreeze\":" << jsonify(keybind::ToggleFreeze) << ",";
                os << "\"SpeedUp\":" << jsonify(keybind::SpeedUp) << ",";
                os << "\"SpeedDown\":" << jsonify(keybind::SpeedDown);
                os << "}";
                return os.str();
            }
            void from_json(const Json &data)
            {
                if (const auto value = data.as<Json::Object>())
                {
                    K(ToggleFreeze)
                    K(SpeedUp)
                    K(SpeedDown)
                }
            }
        }
    }
}
