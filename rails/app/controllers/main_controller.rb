class MainController < ApplicationController
    def index
        file = File.read(Rails.public_path.join('ad.json'))
        data_hash = JSON.parse(file)
        @ads = data_hash['ads']
        @user_count = data_hash['users']
    end
end
