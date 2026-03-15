#pragma once
#include <Model/Arrow.h>
#include <SFML/Graphics.hpp>
#include <cmath>

const float PI = acos(-1);

Arrow::Arrow() {
    body.setFillColor(sf::Color::White);

    head.setPointCount(3);
    head.setFillColor(sf::Color::White);

    updateShape();
}

Arrow::Arrow(const sf::Vector2f& from, const sf::Vector2f& to, const float& thickness, const float& head_length, const float& head_width)
    : from(from), to(to), thickness(thickness), head_length(head_length), head_width(head_width) {
    body.setFillColor(sf::Color::White);

    head.setPointCount(50);
    head.setFillColor(sf::Color::White);

    updateShape();
}

void Arrow::setPosition(const sf::Vector2f& from) {
    this->from = from;
    updateShape();
}

sf::Vector2f Arrow::getPosition() const {
    return from;
}

void Arrow::setTarget(const sf::Vector2f& to) {
    this->to = to;
    updateShape();
}

sf::Vector2f Arrow::getTarget() const {
    return to;
}

void Arrow::setPoints(const sf::Vector2f& from, const sf::Vector2f& to) {
    this->from = from;
    this->to = to;
    updateShape();
}

void Arrow::setThickness(const float& thickness) {
    this->thickness = thickness;
    updateShape();
}

float Arrow::getThickness() const {
    return thickness;
}

void Arrow::setHeadLength(const float& head_length) {
    this->head_length = head_length;
    updateShape();
}

float Arrow::getHeadLength() const {
    return head_length;
}

void Arrow::setHeadWidth(const float& head_width) {
    this->head_width = head_width;
    updateShape();
}

float Arrow::getHeadWidth() const {
    return head_width;
}

void Arrow::setColor(const sf::Color& color) {
    body.setFillColor(color);
    head.setFillColor(color);
}

sf::Color Arrow::getColor() const {
    return body.getFillColor();
}

float Arrow::length(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f Arrow::normalize(const sf::Vector2f& v) {
    float len = length(v);
    if (len == 0.f) return { 0.f, 0.f };
    return v / len;
}

void Arrow::updateShape() {
    sf::Vector2f d = to - from;
    float len = length(d);

    if (len == 0.f) {
        body.setSize({ 0.f, 0.f });

        head.setPoint(0, { 0.f, 0.f });
        head.setPoint(1, { 0.f, 0.f });
        head.setPoint(2, { 0.f, 0.f });
        head.setPosition(from);

        return;
    }

    float actual_head_length = std::min(head_length, len);
    float body_length = std::max(0.f, len - actual_head_length);

    float angle_rad = std::atan2(d.y, d.x);
    float angle_deg = angle_rad * 180.f / PI;

    body.setSize({ body_length, thickness });
    body.setOrigin({ 0.f, thickness / 2.f });
    body.setPosition(from);
    body.setRotation(sf::degrees(angle_deg));

    head.setPointCount(3);
    head.setPoint(0, { 0.f, 0.f });
    head.setPoint(1, { -actual_head_length, -head_width / 2.f });
    head.setPoint(2, { -actual_head_length, head_width / 2.f });
    head.setPosition(to);
    head.setRotation(sf::degrees(angle_deg));
}