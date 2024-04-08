#include "TextComponent.h"
#include <entt.hpp>

std::string SCION_CORE::ECS::TextComponent::to_string()
{
	std::stringstream ss;
	ss <<
		"==== Text Component ==== \n" << std::boolalpha <<
		"Text: " << sTextStr << "\n" <<
		"Font Name: " << sFontName << "\n" <<
		"Padding: " << padding << "\n" <<
		"Wrap: " << wrap << "\n" <<
		"bHidden: " << bHidden << "\n" <<
		"Color: \n\t" <<
			"Red: " << color.r << "\n\t" <<
			"Green: " << color.g << "\n\t" <<
			"Blue: " << color.b << "\n\t" <<
			"Alpha: " << color.a << "\n";

	return ss.str();
}

void SCION_CORE::ECS::TextComponent::CreateLuaTextBindings(sol::state& lua)
{
	lua.new_usertype<TextComponent>(
		"TextComponent",
		"type_id", entt::type_hash<TextComponent>::value,
		sol::call_constructor,
		sol::factories(
			[](const std::string& sFontName, const std::string& sTextStr, SCION_RENDERING::Color color, int padding, float wrap) {
				return TextComponent{
					.sFontName = sFontName,
					.sTextStr = sTextStr,
					.padding = padding,
					.wrap = wrap,
					.color = color
				};
			}
		),
		"textStr", &TextComponent::sTextStr, 
		"fontName", &TextComponent::sFontName,
		"padding", &TextComponent::padding,
		"bHidden", &TextComponent::bHidden,
		"wrap", &TextComponent::wrap,
		"color", &TextComponent::color,
		"to_string", &TextComponent::to_string
	);
}
