import React from 'react';
import { IoIosHome, IoMdSettings } from "react-icons/io";
import { IoLogOut } from "react-icons/io5";

export const SidebarData = [
    {
        title: "Home",
        icon: <IoIosHome />,
        link: "/home"
    },
    {
        title: "Settings",
        icon: <IoMdSettings />,
        link: "/settings"
    },
    {
        title: "Logout",
        icon: <IoLogOut />,
        link: "/logout"
    },
];

