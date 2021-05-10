#include <Graphics/Batch.hpp>
#include <Graphics/Drawer.hpp>
#include <Graphics/Renderer.hpp>

using namespace std;

Batch::~Batch() {

}

void Batch::Refresh() {

}

void Batch::AddDrawer(Drawer *drawer) {
    drawers.insert(drawer);
}

void Batch::RemoveDrawer(Drawer *drawer) {
    drawers.erase(drawer);
}

void Batch::Draw(Renderer *renderer) {
    Refresh();
    for (Drawer *drawer : drawers) {
        drawer->Draw(renderer);
    }
}